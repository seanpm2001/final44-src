h42978
s 00024/00000/00000
d D 7.1 92/06/15 06:28:20 fujita 1 0
c date and time created 92/06/15 06:28:20 by fujita
e
u
U
t
T
I 1
/*
 * Copyright (c) 1992 OMRON Corporation.
 * Copyright (c) 1992 The Regents of the University of California.
 * All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * OMRON Corporation.
 *
 * %sccs.include.redist.c%
 *
 *	%W% (Berkeley) %G%
 */

/*
 * memory.h -- ??? ( This file should be removed )
 * by A.Fujita, MAR-14-1992
 */


struct prgmem {
	u_long pg_start;
	u_long pg_end;
	u_long pg_stack;
};
E 1
