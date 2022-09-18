h09957
s 00002/00002/00044
d D 8.1 93/06/04 17:44:21 bostic 5 4
c 4.4BSD snapshot (revision 8.1); add 1993 to copyright
e
s 00009/00007/00037
d D 5.3 91/04/22 15:53:00 bostic 4 3
c new copyright; att/bsd/shared
e
s 00001/00001/00043
d D 5.2 91/02/24 15:45:46 bostic 3 2
c Add include files to get prototype declarations, and fix bugs found.
e
s 00007/00001/00037
d D 5.1 85/05/07 12:14:55 dist 2 1
c Add copyright notice
e
s 00038/00000/00000
d D 4.1 83/11/10 14:44:22 ralph 1 0
c date and time created 83/11/10 14:44:22 by ralph
e
u
U
t
T
I 2
D 4
/*
 * Copyright (c) 1980 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
E 4
I 4
/*-
D 5
 * Copyright (c) 1980 The Regents of the University of California.
 * All rights reserved.
E 5
I 5
 * Copyright (c) 1980, 1993
 *	The Regents of the University of California.  All rights reserved.
E 5
 *
 * %sccs.include.proprietary.c%
E 4
 */

E 2
I 1
#ifndef lint
static char sccsid[] = "%W% (Berkeley) %G%";
D 2
#endif
E 2
I 2
D 4
#endif not lint
E 4
I 4
#endif /* not lint */
E 4
E 2

/*
 * Displays plot files on an HP7221 plotter.
 * Cloned from bgplot.c and gigiplot.c by Jim Kleckner
 * Thu Jun 30 13:35:04 PDT 1983
D 4
 *  Requires a handshaking program such as hp7221cat to get
 *  the plotter open and ready.
E 4
I 4
 *
 * Requires a handshaking program such as hp7221cat to get the plotter
 * open and ready.
E 4
 */

#include <signal.h>
#include "hp7221.h"

int currentx = 0;
int currenty = 0;
double lowx = 0.0;
double lowy = 0.0;
double scale = 1.0;

openpl()
{
D 3
	int closepl();
E 3
I 3
	void closepl();
E 3

	/* catch interupts */
	signal(SIGINT, closepl);
	currentx = 0;
	currenty = 0;
	printf( "~VR~W" );
	putMBP( 800, 2000 );
	putMBP( 7600, 9600 );
	printf( "~S" );
	putMBP( XMAX, YMAX );
	printf( "vA~*z" );

	space(0,0,XMAX,YMAX);
}
E 1
