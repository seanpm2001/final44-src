/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * Copyright (c) 1988 Carnegie-Mellon University
 * Copyright (c) 1987 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	cons.c,v $
 * Revision 2.6  89/03/09  22:13:17  rpd
 * 	More cleanup.
 * 
 * Revision 2.5  89/02/25  20:12:17  gm0w
 * 	Changes for cleanup.
 * 
 * Revision 2.4  88/12/19  02:52:26  mwyoung
 * 	Fix include file references.
 * 	[88/12/19  00:48:24  mwyoung]
 * 	
 * 	VAX6200: In cnrint, have first character seen on slave, do a 
 * 	move_console to master and then process the char as normal.
 * 	So char's processed at interrupt level will force you back
 * 	to the master.  NOTE:  kdb poll's its characters so it will not
 * 	switch you back.
 * 	[88/12/06            rvb]
 * 
 * Revision 2.3  88/08/10  10:15:53  rvb
 * 	Up delay at end of vaxputc for VAX8800 to 50,000.  This seems to
 * 	work ok and not get ^S's.
 * 
 * 24-Jun-88  Robert Baron (rvb) at Carnegie-Mellon University
 * 	'M' switches 6200 back to master.
 * 
 * 27-May-88  Robert Baron (rvb) at Carnegie-Mellon University
 * 	Changed splx(0x1f) to splhigh().  (which no longer is 0x1f).
 * 
 *  2-May-88  Robert Baron (rvb) at Carnegie-Mellon University
 * 	VAX820: support for slave printout.
 * 
 *
 * 25-May-87  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	8800 support.  The 8800 console is a lost cause.  Many hacks to
 *	get around hardware bugs/bogosities.
 *
 * 15-May-87  Robert Baron (rvb) at Carnegie-Mellon University
 *	Hook for mach_db debugger
 *
 * 29-Dec-86  David Golub (dbg) at Carnegie-Mellon University
 *	Removed last use of mpinc - to set should_exit.  This variable
 *	may need a lock, but it's not tested under one.
 *
 * 17-Dec-86  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	If we are running physical, then always do a vax_putc instead of
 *	the indirect through vputc.  The problem here is that if we are
 *	running relocated, the indirect address is bogus.
 *
 *  7-Dec-86  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Redid console routines to use indirect function pointers (like I
 *	suggested to myself yesterday).  By default, they are set to the
 *	standard Vax console routines.  However, other drivers may set
 *	them to their preference.
 *
 *  6-Dec-86  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Reorganize virtual console routines.  They currently call qvss
 *	the qdss routines.  This should probably all be redone so that
 *	the virtual console routines are function pointers.
 *
 * 14-Nov-86  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Install bug fixes for tabs on 780 console and make cnputc honor
 *	flow control.
 *
 * 14-May-86  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Dispatch to slave console input handling from cnrint instead of
 *	having a separate SCB vector.
 *
 * 26-Jan-86  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Upgraded to 4.3.
 *
 * 20-Nov-85  Mike Accetta (mja) at Carnegie-Mellon University
 *	CMUCS:  Changed cnputc() to check return value from
 *	vputc() to determine whether or not to also write to the
 *	console and cngetc() to check return value to determine whether
 *	or not to also accept console input;  added cnpollc() routine
 *	to set set/clear receive character polling mode.
 *	
 * 25-Aug-85  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Added some simple stuff for slave processing.
 *
 * 26-Jun-85  Mike Accetta (mja) at Carnegie-Mellon University
 *	CMUCS:  created new cngetc() routine also hooked through
 *	new virtual console input function;  changed cnrint() to ignore
 *	receive interrupts when this routine is being used so that it
 *	will receive all characters typed.
 *	MACH_KDB:  hooked cnrint() to kdb_kintr().
 *	[V1(1)]
 *
 * 15-Jun-85  Mike Accetta (mja) at Carnegie-Mellon University
 *	CMUCS:  hooked cnputc() through new virtual console output
 *	function (a la ULTRIX-32).
 *	[V1(1)]
 *
 * 07-May-85  Mike Accetta (mja) at Carnegie-Mellon University
 *	Upgraded to 4.2BSD.  Carried over changes below.
 *	[V1(1)]
 *
 * 30-Mar-83  Mike Accetta (mja) at Carnegie-Mellon University
 *	CMUCS:  Added setting of terminal location in cnopen();
 *	added ncons definition for cdevsw[] (V3.06h).
 *
 */
 
#include <cputypes.h>

#include <mach_kdb.h>
#include <mach_db.h>
/*
 * Copyright (c) 1982, 1986 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)cons.c	7.1 (Berkeley) 6/5/86
 */

/*
 * VAX console driver (and floppy interface)
 */
#include <sys/param.h>
#include <sys/conf.h>
#include <sys/dir.h>
#include <sys/user.h>
#include <sys/proc.h>
#include <sys/ioctl.h>
#include <sys/tty.h>
#include <sys/systm.h>
#include <sys/uio.h>
#include <kern/lock.h>

#include <vax/cpu.h>
#include <vax/cons.h>
#include <vax/mtpr.h>

struct	tty cons;
int	ncons = 1;
int	vax_rpollc = 0;	/* polling for receive characters active flag */
int	cnstart();
int	ttrstrt();
char	partab[];

int	vax_getc(), vax_putc(), vax_pollc();

int	(*vgetc)() = vax_getc;
int	(*vputc)() = vax_putc;
int	(*vpollc)() = vax_pollc;

int	consdone = 1;

/*ARGSUSED*/
cnopen(dev, flag)
	dev_t dev;
{
	register struct tty *tp = &cons;

	tp->t_oproc = cnstart;
	if ((tp->t_state&TS_ISOPEN) == 0) {
		ttychars(tp);
		tp->t_state = TS_ISOPEN|TS_CARR_ON;
		tp->t_flags = EVENP|ECHO|XTABS|CRMOD;
		tp->t_ttyloc.tlc_hostid = TLC_MYHOST;
		tp->t_ttyloc.tlc_ttyid = TLC_CONSOLE;
	}
	if (tp->t_state&TS_XCLUDE && u.u_uid != 0)
		return (EBUSY);
	mtpr(RXCS, mfpr(RXCS)|RXCS_IE);
#if	VAX8800
	if (cpu == VAX_8800)	/* can't enable here due to 8800 bug */
		consdone = 0;	/* so intrs will get started by cnstart. */
	else
#endif	VAX8800
	mtpr(TXCS, mfpr(TXCS)|TXCS_IE);
	return ((*linesw[tp->t_line].l_open)(dev, tp));
}

slave_cnenable()
{
	mtpr(RXCS, mfpr(RXCS)|RXCS_IE);
}

/*ARGSUSED*/
cnclose(dev)
	dev_t dev;
{
	register struct tty *tp = &cons;

	(*linesw[tp->t_line].l_close)(tp);
	ttyclose(tp);
}

/*ARGSUSED*/
cnread(dev, uio)
	dev_t dev;
	struct uio *uio;
{
	register struct tty *tp = &cons;

	return ((*linesw[tp->t_line].l_read)(tp, uio));
}

/*ARGSUSED*/
cnwrite(dev, uio)
	dev_t dev;
	struct uio *uio;
{
	register struct tty *tp = &cons;

	return ((*linesw[tp->t_line].l_write)(tp, uio));
}

/*
 * Got a level-20 receive interrupt -
 * the LSI wants to give us a character.
 * Catch the character, and see who it goes to.
 */
/*ARGSUSED*/
#if	VAX6200
#include <mach/machine.h>
#endif	VAX6200
cnrint(dev)
	dev_t dev;
{
	register int c;
	register struct tty *tp;

	if (vax_rpollc)
		return;
	c = mfpr(RXDB);
	if (cpu_number() != master_cpu) {
#if	VAX6200
		if (cpu == VAX_6200 && machine_info.avail_cpus > 1)
			move_console(master_cpu);
		else {
#endif	VAX6200
		slave_input(c&0177);
		return;
#if	VAX6200
		}
#endif	VAX6200
	}
	if (c&RXDB_ID) {
#if	VAX780
		if (cpu == VAX_780)
			cnrfl(c);
#endif
#if	VAX8800
		if (cpu == VAX_8800)
			nautilus_cin(c);
#endif	VAX8800
		return;
	}
	tp = &cons;
#if	MACH_KDB
#define CNESC	('_'&077)
	{
	    static int escape = 0;

	    if (escape)
	    {
	        escape = 0;
		switch (c&0177)
		{
		    /*
		     *  Escaped escape character.  Use it.
		     */
		    case CNESC:
			break;

		    /*
		     *  Kernel debugger keyboard interrupt.
		     */
		    case 'k':
		    case 'K':
		    case 'K'&077:
#if	MACH_DB
			mdb_open(tp);
#endif	MACH_DB
			kdb_kintr();
			return;

		    /*
		     *  Any other character.  Queue the delayed escape
		     *  character and then the current character (below).
		     */
		    default:
			(*linesw[tp->t_line].l_rint)(CNESC, tp);
			break;
		}
	    }
	    else if ((c&0177) == CNESC)
	    {
		escape++;
		return;
	    }
	}
#endif	MACH_KDB
	(*linesw[tp->t_line].l_rint)(c, tp);
}

slave_input(c)
	register int c;
{
	extern int should_exit[];

	if (c == 'E') {
		should_exit[cpu_number()] = TRUE;
	} else
#if	VAX6200
	if (cpu == VAX_6200)
		if (c == 'M')
			move_console(master_cpu);
		else
			printf("Slave %d active, press 'E' to exit.  Press 'M' to master.\n",
				cpu_number());
	else
#endif	VAX6200
		printf("Slave %d active, press 'E' to exit.\n",
			cpu_number());
}

/*ARGSUSED*/
cnioctl(dev, cmd, addr, flag)
	dev_t dev;
	caddr_t addr;
{
	register struct tty *tp = &cons;
	int error;
 
	error = (*linesw[tp->t_line].l_ioctl)(tp, cmd, addr);
	if (error >= 0)
		return (error);
	error = ttioctl(tp, cmd, addr, flag);
	if (error < 0)
		error = ENOTTY;
	return (error);
}

/*
 * Got a level-20 transmission interrupt -
 * the LSI wants another character.  First,
 * see if we can send something to the typewriter.
 * If not, try the floppy.
 */
/*ARGSUSED*/
cnxint(dev)
	dev_t dev;
{
	register struct tty *tp = &cons;

	consdone++;
	tp->t_state &= ~TS_BUSY;
	if (tp->t_line)
		(*linesw[tp->t_line].l_start)(tp);
	else
		cnstart(tp);
#if	VAX780
	if (cpu==VAX_780 && (tp->t_state & TS_BUSY) == 0)
		conxfl();
#endif
#if	VAX8800
	if (cpu==VAX_8800 && (tp->t_state & TS_BUSY) == 0)
		nautilus_cout();
#endif	VAX8800
}

cnstart(tp)
	register struct tty *tp;
{
	register int c, s;
#if	VAX8800
	register boolean_t	enable = TRUE;
#endif	VAX8800

	s = spl5();
	if (tp->t_state & (TS_TIMEOUT|TS_BUSY|TS_TTSTOP))
		goto out;
	if (tp->t_outq.c_cc <= TTLOWAT(tp)) {
		if (tp->t_state&TS_ASLEEP) {
			tp->t_state &= ~TS_ASLEEP;
			wakeup((caddr_t)&tp->t_outq);
		}
		if (tp->t_wsel) {
			selwakeup(tp->t_wsel, tp->t_state & TS_WCOLL);
			tp->t_wsel = 0;
			tp->t_state &= ~TS_WCOLL;
		}
	}
#if	VAX8800
	/*
	 *	8800 will continue to interrupt unless we disable the
	 *	interrupts.  (HARDWARE BUG).
	 */
	enable = FALSE;
#endif	VAX8800
	if (tp->t_outq.c_cc == 0)
		goto out;

	if (consdone == 0) {
#if	VAX8800
		/*
		 *	Due to the 8800 bug, generate an interrupt
		 *	when ready for the next character.
		 */
		if (cpu == VAX_8800)
			mtpr(TXCS, mfpr(TXCS)|TXCS_IE);
#endif	VAX8800
		return;
	}
	c = getc(&tp->t_outq);
 	if (c <= 0177 || tp->t_flags&(RAW|LITOUT)) {
#if	VAX8800
		while ((mfpr(TXCS)&TXCS_RDY) == 0) ;	/* spin */
#endif	VAX8800
		mtpr(TXDB, c&0xff);
#if	VAX8800
		/*
		 *	Again, due to the 8800 bug, generate an interrupt
		 *	when ready for the next character.  Note, we generate
		 *	the interrupt even if there are no more characters
		 *	so that the TS_BUSY bit will get turned off.
		 */
		if (cpu == VAX_8800)
			enable = TRUE;	/* leave intrs on */
#endif	VAX8800
	}
	else {
		timeout(ttrstrt, (caddr_t)tp, (c&0177));
		tp->t_state |= TS_TIMEOUT;
		goto out;
	}
	consdone = 0;
	tp->t_state |= TS_BUSY;
out:
#if	VAX8800
	/*
	 *	If we are an 8800, see if a state change is necessary.
	 */
	if (cpu == VAX_8800) {
		register int 	state;

		state = mfpr(TXCS);
		if ((state & TXCS_IE) != enable) {	/* intrs on? */
			if (enable)
				mtpr(TXCS, state | TXCS_IE);
			else
				mtpr(TXCS, state & ~TXCS_IE);
		}
	}
#endif	VAX8800
	splx(s);
}

/*
 * Print a character on console.
 */
cnputc(c)
	int c;
{

	if ((mfpr(MAPEN) & 1) == 0)
		vax_putc(c);
	else
		(*vputc)(c);
}

/*
 *  Set/clear receive character polling mode
 */

cnpollc(flag)
	int	flag;
{
	(*vpollc)(flag);
}

/*
 *	Get a character from the console.
 */
cngetc()
{
	register c;

	c = (*vgetc)();
	if (c=='\r')
		c = '\n';
	cnputc(c);
	return(c);
}

/*
 * Print a character on console.
 * Attempts to save and restore device
 * status.
 */
vax_putc(c)
	register int c;
{
	register int s, timo;

#if	VAX820
	if (cpu == VAX_820 && cpu_number() != master_cpu) {
		rxcd_send(master_cpu, c&0xff);	/* I may need to delay */
		if (c == '\n')
			vax_putc('\r');
		return;
	}
#endif	VAX820

 	if (mfpr(RXCS)&RXCS_DONE && (mfpr(RXDB)&0177) == CTRL(S))
 		while ((mfpr(RXCS)&RXCS_DONE) == 0 ||
 		       (mfpr(RXDB)&0177) != CTRL(Q))
 			;
	timo = 30000;
	/*
	 * Try waiting for the console tty to come ready,
	 * otherwise give up after a reasonable time.
	 */
	while ((mfpr(TXCS)&TXCS_RDY) == 0)
		if(--timo == 0)
			break;
	if (c == 0)
		return;
	s = mfpr(TXCS);
	mtpr(TXCS, 0);
	mtpr(TXDB, c&0xff);
	if (c == '\n')
		vax_putc('\r');
	vax_putc(0);
	mtpr(TXCS, s);
#if	VAX8800
	if (cpu == VAX_8800) {
		for (timo = 0; timo < 50000; timo++) /* 8800 temp */
			;
	}
#endif	VAX8800
}

/*
 *  Get character from console.
 */

vax_getc()
{
	register c;
	register int ipl;
	/*
	 *  Make sure we loop at or above the console interrupt priority
	 *  otherwise we can get into an infinite loop processing console
	 *  receive interrupts which will never be handled because the
	 *  interrupt routine is short-circuited while we are doing direct
	 *  input.
	 */
	ipl = splhigh();
	if (ipl < 0x14)
		(void)splx(0x14);
	else
		(void)splx(ipl);
	/*
	 *  Get a character from the virtual console if it exists or failing
	 *  that from the standard console.  A -1 return from vgetc() indicates
	 *  that the virtual console does not exist or does exist with no
	 *  character available while being duplicated with the standard
	 *  console.  A 0 return indicates that the virtual console exists with
	 *  no character available but that it is being used exclusively as the
	 *  console.
	 */
	while ((mfpr(RXCS)&RXCS_DONE) == 0 || (c=mfpr(RXDB)&0177) < 0)
		;
	if (c == '\r')
		c = '\n';
	(void)splx(ipl);
	return (c);
}


/*
 *  Set/clear receive character polling mode
 */

vax_pollc(flag)
{
	if (flag)
		vax_rpollc++;
	else
		vax_rpollc--;
}
