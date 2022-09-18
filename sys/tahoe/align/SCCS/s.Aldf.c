h12158
s 00011/00001/00019
d D 7.1 90/12/06 13:36:36 bostic 3 2
c add Berkeley specific copyright notice
e
s 00001/00001/00019
d D 1.2 90/12/04 22:39:08 bostic 2 1
c /sys/tahoealign -> /sys/tahoe/align
e
s 00020/00000/00000
d D 1.1 86/07/20 11:14:19 sam 1 0
c date and time created 86/07/20 11:14:19 by sam
e
u
U
t
T
I 1
D 3
/*	%M%	%I%	%E%	*/
E 3
I 3
/*-
 * Copyright (c) 1986 The Regents of the University of California.
 * All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Computer Consoles Inc.
 *
 * %sccs.include.redist.c%
 *
 *	%W% (Berkeley) %G%
 */
E 3

D 2
#include "../tahoealign/align.h"
E 2
I 2
#include "align.h"
E 2
ldf(infop)	process_info *infop;
/*
/*	Load a float operand into accumulator.
/*
/*************************************************/
{

	register struct oprnd *oprnd_pnt;

	oprnd_pnt = operand(infop,0);
	if ( reserved( oprnd_pnt->data ) ) 
		exception(infop, ILL_OPRND);
	if ( (oprnd_pnt->data & 0xff800000) == 0 ) acc_high = 0;
	else acc_high = oprnd_pnt->data ;
	psl &= ~PSL_DBL;
	infop->acc_dbl = 0;
}
E 1
