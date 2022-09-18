h09075
s 00002/00002/00029
d D 8.1 93/06/04 12:23:41 bostic 2 1
c 4.4BSD snapshot (revision 8.1); add 1993 to copyright
e
s 00031/00000/00000
d D 5.1 90/05/12 15:47:36 bostic 1 0
c date and time created 90/05/12 15:47:36 by bostic
e
u
U
t
T
I 1
/*-
D 2
 * Copyright (c) 1990 The Regents of the University of California.
 * All rights reserved.
E 2
I 2
 * Copyright (c) 1990, 1993
 *	The Regents of the University of California.  All rights reserved.
E 2
 *
 * This code is derived from software contributed to Berkeley by
 * the Systems Programming Group of the University of Utah Computer
 * Science Department.
 *
 * %sccs.include.redist.c%
 */

#if defined(LIBC_SCCS) && !defined(lint)
	.asciz "%W% (Berkeley) %G%"
#endif /* LIBC_SCCS and not lint */

#include "DEFS.h"

/*
 * double ldexp(val, exp)
 * returns: val * (2**exp), for integer exp
 */
ENTRY(ldexp)
	fmoved		sp@(4),fp0
	fbeq		Ldone
	ftwotoxl	sp@(12),fp1
	fmulx		fp1,fp0
Ldone:
	fmoved		fp0,sp@-
	movel		sp@+,d0
	movel		sp@+,d1
	rts
E 1
