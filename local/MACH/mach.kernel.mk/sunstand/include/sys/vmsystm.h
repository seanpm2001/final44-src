/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	vmsystm.h,v $
 * Revision 2.2  89/07/12  00:17:38  jjc
 * 	Picked up from SunOS 4.0.
 * 	[89/07/03            jjc]
 * 
 */
/*	@(#)vmsystm.h 2.6 88/02/08 SMI; from UCB 4.3 81/04/23	*/

/*
 * Miscellaneous virtual memory subsystem variables and structures.
 */

#ifdef KERNEL
int	freemem;		/* remaining blocks of free memory */
int	avefree;		/* moving average of remaining free blocks */
int	avefree30;		/* 30 sec (avefree is 5 sec) moving average */
int	deficit;		/* estimate of needs of new swapped in procs */
int	nscan;			/* number of scans in last second */
int	multprog;		/* current multiprogramming degree */
int	desscan;		/* desired pages scanned per second */

/* writable copies of tunables */
int	maxpgio;		/* max paging i/o per sec before start swaps */
int	maxslp;			/* max sleep time before very swappable */
int	lotsfree;		/* max free before clock freezes */
int	minfree;		/* minimum free pages before swapping begins */
int	desfree;		/* no of pages to try to keep free via daemon */
int	saferss;		/* no pages not to steal; decays with slptime */
#endif

/*
 * Fork/vfork accounting.
 */
struct	forkstat
{
	int	cntfork;
	int	cntvfork;
	int	sizfork;
	int	sizvfork;
};
#ifdef KERNEL
struct	forkstat forkstat;
#endif
