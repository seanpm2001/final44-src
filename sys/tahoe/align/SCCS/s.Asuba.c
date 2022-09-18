h02561
s 00011/00001/00015
d D 7.1 90/12/06 13:37:35 bostic 3 2
c add Berkeley specific copyright notice
e
s 00001/00001/00015
d D 1.2 90/12/04 22:40:04 bostic 2 1
c /sys/tahoealign -> /sys/tahoe/align
e
s 00016/00000/00000
d D 1.1 86/07/20 11:14:46 sam 1 0
c date and time created 86/07/20 11:14:46 by sam
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
suba(infop)	process_info *infop;
/*
/*	Subtract address.
/*
/****************************/
{
	register int data0, data1, result;

	data0 = operand(infop,0)->data;
	data1 = operand(infop,1)->data;
	result = data1 - data0;
	write_back (infop,result, operand(infop,1));
}
E 1
