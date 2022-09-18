/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	winlock.c,v $
 * Revision 2.9  89/07/12  01:27:44  jjc
 * 	Modified to include files from machine/ rather than just sun3/ since we
 * 	have sun4/ now too.
 * 	[89/07/05  21:14:24  jjc]
 * 
 * Revision 2.8  89/03/09  22:01:12  rpd
 * 	More cleanup.
 * 
 * Revision 2.7  89/02/25  20:08:26  gm0w
 * 	Added new copyright and fixed includes as part of kernel cleanup.
 * 	[89/02/12            jjc]
 * 
 * Revision 2.6  88/12/08  15:39:04  mikeg
 * 	Modified to allow Suntools to run under Mach.
 * 	[88/08/17  17:03:04  mikeg]
 * 
 * Revision 2.4  88/10/18  00:34:39  mwyoung
 * 	Forward declarations of static's to make gcc happy:
 * 	 	static favor_pid_and_children();
 * 	[88/10/13            rvb]
 * 
 * Revision 2.3  88/08/24  02:21:30  mwyoung
 * 	Corrected include file references.
 * 	[88/08/22            mwyoung]
 * 
 * 29-Apr-87  David Golub (dbg) at Carnegie-Mellon University
 *	Gutted this module for MACH.  I doubt that
 *	any of these functions are called when running X.  If they are,
 *	or if someone insists on running SunWindows instead, they'll
 *	have to be rewritten.
 *
 * 23-Sep-86  Jonathan J. Chew (jjc) at Carnegie-Mellon University
 *	Modified for Mach.
 *
 */
#ifndef	lint
static	char sccsid[] = "@(#)winlock.c 1.1 86/09/25";
#endif

/*
 * Copyright (c) 1986 by Sun Microsystems, Inc.
 */

/*
 * SunWindows locking primitives.
 */

#include <sunwindowdev/wintree.h>
#include <sys/vmmac.h>	/* for kmxtob macro */
#ifdef	MACH
#include <vm/vm_kern.h>
#include <mach/machine/kern_return.h>
#include <mach/boolean.h>
#include <kern/thread.h>
#include <mach/thread_info.h>
#include <kern/thread_swap.h>
#else	MACH
#include <machine/pte.h>	/* for Usrptmap extern */
#endif	MACH
#include <sys/kernel.h>	/* for rmalloc declaration */

static favor_pid_and_children();

int	wlok_time_workaround = 1;
				/* Temp:workaround for 0 u/stime after uaccess*/

extern	int	ws_check_lock;		/* Check locks every #of hz if pid */
extern	int	ws_no_pid_check_lock;	/* Check locks every #of hz if no pid */
int	ws_set_pri;			/* Reschedule every time set priority */
int	ws_set_nice = 1;		/* Set nice every time lock */
int	ws_nice = NZERO;		/* Nice used when ws_set_nice */

extern	struct	timeval ws_check_time;	/* Check locks every this # usec */
int	ws_set_favor = 0;		/* Control favoring feature */

/*
 * ws_lock_limit is the process virtual time limit for a data or display
 * lock.  The check for this amount of virtual time doesn't start for
 * wlock->lok_max_time amount of real time after the lock is set.  This is
 * done to avoid the overhead of mapping the user structure for normal
 * short lock intervals.
 */
#define WS_LOCK_LIMIT_SEC	2	/* Break locks in this # of seconds */
#define WS_LOCK_LIMIT_USEC	0	/* plus this # of micro seconds */
struct	timeval ws_lock_limit = {WS_LOCK_LIMIT_SEC, WS_LOCK_LIMIT_USEC};

/*
 * Generic access control
 */
int
wlok_waitunlocked(wlock)
	Winlock *wlock;
{
#ifdef	MACH
	thread_t thread;

	thread = current_thread();
	while ((wlock->lok_flags) && (*wlock->lok_flags & wlock->lok_bit) &&
	    (wlock->lok_pid != proc[thread->task->proc_index].p_pid))
#else	MACH
	while ((wlock->lok_flags) && (*wlock->lok_flags & wlock->lok_bit) &&
	    (wlock->lok_pid != u.u_procp->p_pid))
#endif	MACH
#ifdef	MACH
		if (tsleep((caddr_t)wlock->lok_wakeup, LOCKPRI, 0) == TS_SIG)
#else	MACH
		if (sleep((caddr_t)wlock->lok_wakeup, LOCKPRI|PCATCH))
#endif	MACH
			return (-1);
	return (0);
}

int	running_check_count = 0;

wlok_running_check(wlock)
	Winlock *wlock;
{
	char stat;

#ifdef	MACH
	thread_t thread;

	running_check_count++;
	/* lok_proc may not be set yet */
	if (!wlock->lok_proc)
		return;

	thread = wlock->lok_proc->thread;
	if ( (((thread->state&TH_SUSP) != 0) && (thread->interruptible) && ((thread->state&TH_RUN) == 0)) ||
	    (wlock->lok_proc->p_pid != wlock->lok_pid)) {
		if (!(wlock->lok_options & WLOK_SILENT))
			printf(
			"Window %s lock broken because process %D not running\n"
			, wlock->lok_string, wlock->lok_pid);
		wlok_forceunlock(wlock);
	}
#else	MACH
	/* lok_proc may not be set yet */
	if (!wlock->lok_proc)
		return;

	stat = wlock->lok_proc->p_stat;
	if ((stat != SRUN && stat != SSLEEP) ||
	    (wlock->lok_proc->p_pid != wlock->lok_pid)) {
		if (!(wlock->lok_options & WLOK_SILENT))
			printf(
			"Window %s lock broken because process %D not running\n"
			, wlock->lok_string, wlock->lok_pid);
		wlok_forceunlock(wlock);
	}
#endif	MACH
}

int	lockcheck_count = 0;

wlok_lockcheck(wlock, poll_rate, charge_sleep_against_limit)
	Winlock *wlock;
	int	poll_rate;
	int	charge_sleep_against_limit;
{
	extern	struct proc *pfind();
	register struct proc *process;
	register int pid;
	struct	timeval utime, stime;
	int pri;

	/*lockcheck_count++;*/

#ifdef	MACH
	thread_t thread;
	thread_basic_info_data_t thinfo;
	thread_basic_info_t infoptr;
        unsigned int info_count = THREAD_BASIC_INFO_COUNT;

	wlock->lok_time += poll_rate;
	if (wlock->lok_time <= wlock->lok_max_time)
		return;
	pri = spl5();	/* Groping here.  See uaccess for problem */
	pid = wlock->lok_pid;
	/* See if process still exists */
	process = pfind(pid);
	if (process == (struct proc *)0) {
		/* Process does not exist */
		if (!(wlock->lok_options & WLOK_SILENT))
			printf(
			"Window %s lock broken after process %D went away\n",
			    wlock->lok_string, pid);
		goto Force;
	}
	/* See if process is running */
	thread = process->thread;
	if (thread->state & TH_RUN == 0) {
		/* See if process is blocked on a non-interruptible device */
		if (thread->state & TH_WAIT) {
		  if (thread->interruptible == FALSE) {
		      /* Let it pass because will awake "soon" */
		  } else {
		   if (charge_sleep_against_limit) {
			/*
			 * Don't start timing process until gets loaded
			 * into core.
			 */
			if ( ( thread->state & TH_SWAPPED ) == 0 ) {
				/*
				 * Subtract ws_check_time from limit to give
				 * affect of charging as real time elasped.
				 */
				if (timercmp((&wlock->lok_limit),
				    (&ws_check_time), >)){
					timevalsub(&wlock->lok_limit,
					    &ws_check_time);
				} else {
					timerclear(&wlock->lok_limit);
				}
			}
		   } else {
		      /* Only print message if not being traced (debugged) */
		      if (process->p_stat != SSTOP) {
			if (!(wlock->lok_options & WLOK_SILENT))
		           printf(
		           "Window %s lock broken because process %D blocked\n",
		               wlock->lok_string, pid);
		      }
		      goto Force;
		   }
		  }
		} else {
		    if (!(wlock->lok_options & WLOK_SILENT))
		      printf(
		      "Window %s lock broken because process %D not running\n",
		      wlock->lok_string, pid);
		    goto Force;
		}
	}
	/*
	 * Don't start timing process until gets loaded into core.
	 */
	if ( thread->state & TH_SWAPPED )
		goto Done;
	else
		thread->swap_state = TH_SW_UNSWAPPABLE;
	infoptr = &thinfo;
	thread_info( thread,THREAD_BASIC_INFO,(thread_info_t)infoptr,&info_count );
	if (!timerisset(&wlock->lok_utime) || !timerisset(&wlock->lok_stime)) {
		/* Save when started checking (a lie for workaround) */
		wlock->lok_utime.tv_sec = infoptr->user_time.seconds;
		wlock->lok_utime.tv_usec = infoptr->user_time.microseconds;
		wlock->lok_stime.tv_sec = infoptr->system_time.seconds;
		wlock->lok_stime.tv_usec = infoptr->system_time.microseconds;
		if (!timerisset(&wlock->lok_utime) ||
		    !timerisset(&wlock->lok_stime))
			goto Done;
	}
	/* Subtract saved time from current time */
	utime.tv_sec = infoptr->user_time.seconds;
	utime.tv_usec = infoptr->user_time.microseconds;
	timevalsub(&utime, &wlock->lok_utime);
	stime.tv_sec = infoptr->system_time.seconds;
	stime.tv_usec = infoptr->system_time.microseconds;
	timevalsub(&stime, &wlock->lok_stime);
	/* See if combined time exceeds threshold */
	timevaladd(&utime, &stime);
	if (wlock->lok_timeout_action &&
	 ((!timerisset(&wlock->lok_limit)) ||
	 timercmp((&utime), (&wlock->lok_limit), >))) {
	  /* Lock timed out */
	  if (!(wlock->lok_options & WLOK_SILENT))
	   printf(
	   "Window %s lock broken after time limit exceeded by pid %D\n"
	      , wlock->lok_string, pid);
	  wlock->lok_timeout_action(wlock);
	  goto Force;
	}

#else	MACH

	wlock->lok_time += poll_rate;
	if (wlock->lok_time <= wlock->lok_max_time)
		return;
	pri = spl5();	/* Groping here.  See uaccess for problem */
	pid = wlock->lok_pid;
	/* See if process still exists */
	process = pfind(pid);
	if (process == (struct proc *)0) {
		/* Process does not exist */
		if (!(wlock->lok_options & WLOK_SILENT))
			printf(
			"Window %s lock broken after process %D went away\n",
			    wlock->lok_string, pid);
		goto Force;
	}
	/* See if process is running */
	if (process->p_stat != SRUN) {
		/* See if process is blocked on a non-interruptible device */
		if (process->p_stat == SSLEEP) {
		  if (process->p_pri < PZERO) {
		      /* Let it pass because will awake "soon" */
		  } else {
		   if (charge_sleep_against_limit) {
			/*
			 * Don't start timing process until gets loaded
			 * into core (see SLOAD below).
			 */
			if (process->p_flag & SLOAD) {
				/*
				 * Subtract ws_check_time from limit to give
				 * affect of charging as real time elasped.
				 */
				if (timercmp((&wlock->lok_limit),
				    (&ws_check_time), >)){
					timevalsub(&wlock->lok_limit,
					    &ws_check_time);
				} else {
					timerclear(&wlock->lok_limit);
				}
			}
		   } else {
		      /* Only print message if not being traced (debugged) */
		      if (process->p_stat != SSTOP) {
			if (!(wlock->lok_options & WLOK_SILENT))
		           printf(
		           "Window %s lock broken because process %D blocked\n",
		               wlock->lok_string, pid);
		      }
		      goto Force;
		   }
		  }
		} else {
		    if (!(wlock->lok_options & WLOK_SILENT))
		      printf(
		      "Window %s lock broken because process %D not running\n",
		      wlock->lok_string, pid);
		    goto Force;
		}
	}
	/*
	 * Don't start timing process until gets loaded into core.
	 */
	if ((~process->p_flag) & SLOAD)
		goto Done;
	else
		/*
		 * We may have set the SULOCK bit already but go ahead and
		 * do it again.  When it finally gets into core then it should
		 * stay put.
		 *
		 * There is a 2.0 bug in which the scheduler can turn off the
		 * SULOCK.  If this happens then we just turn it on again.
		 */
		process->p_flag |= SULOCK;
	/*
	 * See if have mapped in user struct yet to actually do the process
	 * virtual time tracking.
	 */
	if (wlock->lok_user == (struct user *)0) {
		/* Map user struct so can keep track of process virtual time */
		if (wlock->lok_user_addr == 0)
			goto Force;
		wlock->lok_user = (struct user *)kmxtob(wlock->lok_user_addr);
		/*
		 * Warning: uaccess doesn't have utime and stime set when
		 * SNOMV bit on.  This occurs when parent(?) in a vfork.
		 */
		uaccess(process, &Usrptmap[wlock->lok_user_addr],
		    wlock->lok_user);
		/* Save when started checking */
		wlock->lok_utime = wlock->lok_user->u_ru.ru_utime;
		wlock->lok_stime = wlock->lok_user->u_ru.ru_stime;
	} else {
		if (wlok_time_workaround &&
		  (!timerisset(&wlock->lok_utime) ||
		   !timerisset(&wlock->lok_stime))) {
			/* Save when started checking (a lie for workaround) */
			wlock->lok_utime = wlock->lok_user->u_ru.ru_utime;
			wlock->lok_stime = wlock->lok_user->u_ru.ru_stime;
			if (!timerisset(&wlock->lok_utime) ||
			    !timerisset(&wlock->lok_stime))
				goto Done;
		}
		/* Subtract saved time from current time */
		utime = wlock->lok_user->u_ru.ru_utime;
		timevalsub(&utime, &wlock->lok_utime);
		stime = wlock->lok_user->u_ru.ru_stime;
		timevalsub(&stime, &wlock->lok_stime);
		/* See if combined time exceeds threshold */
		timevaladd(&utime, &stime);
		if (wlock->lok_timeout_action &&
		 ((!timerisset(&wlock->lok_limit)) ||
		 timercmp((&utime), (&wlock->lok_limit), >))) {
		  /* Lock timed out */
		  if (!(wlock->lok_options & WLOK_SILENT))
		   printf(
		   "Window %s lock broken after time limit exceeded by pid %D\n"
		      , wlock->lok_string, pid);
		  wlock->lok_timeout_action(wlock);
		  goto Force;
		}
	}
#endif	MACH
Done:
	/* Reset time until next check */
	wlock->lok_time = 0;
	(void) splx(pri);
	return;
Force:
	(void) splx(pri);
	wlok_forceunlock(wlock);
	return;
}

wlok_forceunlock(wlock)
	Winlock *wlock;
{

	if (wlock->lok_count)
		*(wlock->lok_count) = 1;
	if (wlock->lok_force_action)
		wlock->lok_force_action(wlock);
	wlok_unlock(wlock);
}

/* Most callers will use u.u_procp for proc */
wlok_setlock(wlock, flags, lockbit, lock_count, process)
	register Winlock *wlock;
	int	*flags;
	int	lockbit;
	int	*lock_count;
	register struct	proc *process;
{

#ifdef	MACH
	thread_t thread;
#endif	MACH

	/* If the lock bit is already set,
	 * assume that whoever set it
	 * will set the count also.
	 */
	if (!(*flags & lockbit)) {
		*lock_count = 1;
	}
	*flags |= lockbit;
	wlock->lok_flags = flags;
	wlock->lok_bit = lockbit;
	wlock->lok_count = lock_count;
	wlock->lok_time = 0;
	wlock->lok_limit = ws_lock_limit;	/* Default, can override */
	if (process) {
	    wlock->lok_max_time = ws_check_lock;
	    wlock->lok_pid = process->p_pid;
	    wlock->lok_proc = process;
	    /* Pin process so wouldn't swap (only if currently loaded) */
#ifdef	MACH
	    thread = process->thread;
	    if ( ( thread->state & TH_SWAPPED ) == 0 )
		    thread->swap_state = TH_SW_UNSWAPPABLE;
#else	MACH
	    if (process->p_flag & SLOAD)
		    process->p_flag |= SULOCK;
#endif	MACH
	    if (ws_set_nice) {
		/* Boost priority */
		wlock->lok_nice = process->p_nice;
		process->p_nice = ws_nice;
#ifdef	MACH
	/* We could insert some Mach code here to change the priority of the
	   thread, but it appears to be pointless, since ws_set_pri is 
	   initialized to zero and never set to a nonzero value. */
#else	MACH
		if (ws_set_pri)
			(void) setpri(process);
#endif	MACH
	    }
	} else
	    wlock->lok_max_time = ws_no_pid_check_lock;
	/* Will set up rest of structure when first check lock */
}

wlok_unlock(wlock)
	register Winlock *wlock;
{
	register struct	proc *process;
	caddr_t wakeup_handle;
#ifdef	MACH
	thread_t th;
#endif	MACH

	if ((wlock->lok_flags) && (*wlock->lok_flags & wlock->lok_bit)) {
		if (!(wlock->lok_count) || (*(wlock->lok_count) <= 1)) {
			if (!(wlock->lok_count))
			    printf("Warning: window lock count pointer is NULL\n");

			/* Reset lock state */
			*wlock->lok_flags &= ~wlock->lok_bit;
			/*
			 * Reset process if process ids match and no other
			 * locks outstanding (they all should be on the same
			 * pid).
			 */
			process = wlock->lok_proc;
			if (process && (process->p_pid == wlock->lok_pid)) {
				/* see if this process has any other locks */
				if (!wlock->lok_other_check ||
				    !(wlock->lok_other_check)(wlock->lok_client)) {
					if (ws_set_nice) {
					    process->p_nice = wlock->lok_nice;
#ifdef	MACH
	/* We could insert some Mach code here to change the priority of the
	   thread, but it appears to be pointless, since ws_set_pri is 
	   initialized to zero and never set to a nonzero value. */
					}
					process->thread->swap_state = TH_SW_IN;
				}
#else	MACH
					    if (ws_set_pri)
						(void) setpri(process);
					}
					process->p_flag &= ~SULOCK;
				}
#endif	MACH
			}
			/* Do lock specific action */
		  	if (wlock->lok_unlock_action)
		  		wlock->lok_unlock_action(wlock);
			/*
			 * Zero wlock after noting who to wakeup,
			 * but preserve user_addr.
			 */
			wakeup_handle = wlock->lok_wakeup;
			wlok_clear(wlock);
			wakeup((caddr_t)wakeup_handle);
		} else
			*(wlock->lok_count) -= 1;
	}
}

int	clear_count = 0;

wlok_clear(wlock)
	register Winlock *wlock;
{
	long user_addr;

#ifdef	MACH
	clear_count++;
	if (wlock->lok_count)
		*(wlock->lok_count) = 0;
	bzero((caddr_t)wlock, sizeof (Winlock));
#else	MACH
	/*
	 * Zero wlock but preserve user_addr.
	 */
	user_addr = wlock->lok_user_addr;
	if (wlock->lok_count)
		*(wlock->lok_count) = 0;
#ifdef	VAC
	/*
	 * Lok_user != 0 implies that uaccess() was called.
	 * The mapping from lok_user to the physical address
	 * of this u becomes invalid after bzero().
	 */
	if (wlock->lok_user != (struct user *)0)
		vac_pageflush((caddr_t)(wlock->lok_user));
#endif
	bzero((caddr_t)wlock, sizeof (Winlock));
	wlock->lok_user_addr = user_addr;
#endif	MACH
}

int	done_count = 0;
/* Release virtual memory for locks if allocated */
wlok_done(wlock)
	register Winlock *wlock;
{
#ifdef	MACH
	done_count++;
#else	MACH
	if (wlock->lok_user_addr) {
#ifdef	VAC
		/*
		 * Lok_user != 0 implies that uaccess() was called.
		 * The mapping from lok_user to the physical address
		 * of this u is invalid after rmfree().
		 */
		if (wlock->lok_user != (struct user *)0)
			vac_pageflush((caddr_t)(wlock->lok_user));
#endif
		rmfree(kernelmap, (long)UPAGES, wlock->lok_user_addr);
	}
#endif	MACH
}

int	init_count = 0;
/*
 * Allocate vm so can map user struct later so can keep track of
 * process virtual time.
 */
wlok_init(wlock)
	register Winlock *wlock;
{
#ifdef	MACH
	init_count++;
#else	MACH
	if ((wlock->lok_user_addr = rmalloc(kernelmap, (long)UPAGES)) == 0) {
		/*FAIL;*/
		printf("win rmalloc failed in wlok_lockcheck\n");
		return;
	}
#endif	MACH
}

/*
 * Ws_favor will boost w->w_pid's favor if ws->ws_favor_pid is not
 * equal to w->w_pid.  A null w means that no process is in favor;
 * ws->ws_favor_pid will be zeroed in this case.
 */
ws_favor(ws, w)
	register Workstation *ws;
	register Window *w;
{
	if (w == WINDOW_NULL) {
		ws->ws_favor_pid = 0;
	} else {
		if ((w->w_pid != ws->ws_favor_pid) && ws_set_favor) {
			favor_pid_and_children(w->w_pid);
			ws->ws_favor_pid = w->w_pid;
		}
	}
}

/*
 * Process pid_to_favor now is the interactive process.
 * Adjust it's favor and it's children's favor up.
 * Adjust old process(es) (if any) favor down.
 *
 * The correct way to do this is to recursively favor all the children
 * of the tool that has the input focus.  But we know that the interesting
 * case is the C shell and its children.  Csh makes each of its children
 * a process group leader, and their children in turn keep that process
 * group.  It would be nice if we knew which was the foreground process
 * group, but the controlling pty is pointed by the u area which is not
 * only hard to get to, it may be swapped out.  So we punt and do all
 * the children.
 */

#define FAVOR(p)	((p)->p_flag |= SFAVORD)

#define UNFAVOR(p)	((p)->p_flag &= ~SFAVORD)

#define ISFAVORED(p)	((p)->p_flag & SFAVORD)

int	favor_debug;

int	favor_count = 0;

static
favor_pid_and_children(pid_to_favor)
	int pid_to_favor;
{
	register struct proc *p, *pp;
	int w_pid, sh_pid, pgrp;

#ifdef	MACH
	/*
	 *	Punt for now - we don't have much of a scheduler
	 *	anyway
	 */
	favor_count++;
#else	MACH
	p = pfind(pid_to_favor);
	if (favor_debug)
		printf("%D favored (%X)\n", pid_to_favor, p);
	if (p == 0) {
		return;
	}
	FAVOR(p);
	w_pid = p->p_pid;
	sh_pid = 0;
	for (p = proc; p < procNPROC; p++) {
		/*
		 * Take away niceness from processes that got
		 * it before.
		 */
		if (ISFAVORED(p) && p->p_pid != w_pid) {
			UNFAVOR(p);
		}
		/*
		 * Check for direct child of window process.
		 */
		if (p->p_ppid == w_pid) {
			FAVOR(p);
			sh_pid = p->p_pid;
		}
	}
	if (sh_pid == 0) {
		return;
	}
	/*
	 * Assuming each child of the shell is a process group leader,
	 * do each process group.
	 */
	for (p = proc; p < procNPROC; p++) {
		if (p->p_ppid == sh_pid) {	/* child of shell */
			if (!ISFAVORED(p)) {	/* got it already? */
				FAVOR(p);	/* no, favor it and pgrp */
				pgrp = p->p_pgrp;
				for (pp = proc; pp < procNPROC; pp++) {
					if (pp->p_pgrp == pgrp) {
						FAVOR(pp);
					}
				}
			}
		}
	}
#endif	MACH
}
