h12659
s 00002/00002/00057
d D 8.1 93/06/11 14:44:29 bostic 3 2
c 4.4BSD snapshot (revision 8.1); add 1993 to copyright
e
s 00019/00028/00040
d D 5.2 93/06/11 14:43:54 bostic 2 1
c integrate into 4.4BSD
e
s 00068/00000/00000
d D 5.1 93/06/10 11:09:45 bostic 1 0
c date and time created 93/06/10 11:09:45 by bostic
e
u
U
t
T
I 1
D 2
/* vi: set tabstop=4 : */

/*
 * Configuration
E 2
I 2
/*-
D 3
 * Copyright (c) 1993 The Regents of the University of California.
 * All rights reserved.
E 3
I 3
 * Copyright (c) 1993
 *	The Regents of the University of California.  All rights reserved.
E 3
 *
 * This code is derived from software contributed to Berkeley by
 * Barry Brachman.
 *
 * %sccs.include.redist.c%
 *
 *	%W% (Berkeley) %G%
E 2
 */

D 2
#define BSD42			1	/* BSD 4.[23] */
/* #define ATARI			1	/* Atari ST */
/* #define SYSV				1	/* System V */

/*
 * If you don't have random() and srandom() #define what you have
 */
/* #define random			lrand48	/* Sys V */
/* #define srandom			srand48	/* Sys V */

/* #define random			rand	/* Not good but generally available */
/* #define srandom(x)		srand((int) x)	/* */

#define TIMER			1	/* Use tty polling (see timer.c) */
E 2
#define LOADDICT		1	/* Load the dictionary for speed */

/*
D 2
 * Locations for the dictionary (generated by mkdict), index (generated by
 * mkindex), and helpfile
E 2
I 2
 * Locations for the dictionary (generated by mkdict),
 * index (generated by mkindex), and helpfile
E 2
 */
D 2
#define DICT			"/grads/brachman/lib/bog/dict"
#define DICTINDEX		"/grads/brachman/lib/bog/dictindex"
#define HELPFILE		"/grads/brachman/lib/bog/helpfile"
E 2
I 2
#define DICT			"/usr/share/games/boggle/dictionary"
#define DICTINDEX		"/usr/share/games/boggle/dictindex"
#define HELPFILE		"/usr/share/games/boggle/helpfile"
E 2

/*
 * The theoretical maximum for MAXWORDLEN is ('a' - 1) == 96
 */
D 2
#define MAXWORDLEN		40		/* Maximum word length */
#define MAXPWORDS		200		/* Maximum number of player's words */
#define MAXMWORDS		200		/* Maximum number of machine's words */
E 2
I 2
#define MAXWORDLEN		40	/* Maximum word length */
#define MAXPWORDS		200	/* Maximum number of player's words */
#define MAXMWORDS		200	/* Maximum number of machine's words */
E 2
#define MAXPSPACE		2000	/* Space for player's words */
#define MAXMSPACE		4000	/* Space for machines's words */

#define MAXCOLS			20

/*
 * The following determine the screen layout
 */
#define PROMPT_COL		20
#define PROMPT_LINE		2

#define BOARD_COL		0
#define BOARD_LINE		0

#define SCORE_COL		20
#define SCORE_LINE		0

#define LIST_COL		0
#define LIST_LINE		10

#define TIMER_COL		20
#define TIMER_LINE		2

/*
 * Internal dictionary index
 * Initialized from the file created by mkindex
 */
struct dictindex {
D 2
    long start;
E 2
I 2
	long start;
E 2
	long length;
};
D 2

E 2
E 1
