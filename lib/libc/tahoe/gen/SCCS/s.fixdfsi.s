h47096
s 00002/00002/00015
d D 8.1 93/06/04 13:17:09 bostic 2 1
c 4.4BSD snapshot (revision 8.1); add 1993 to copyright
e
s 00017/00000/00000
d D 5.1 92/07/19 14:34:59 bostic 1 0
c date and time created 92/07/19 14:34:59 by bostic
e
u
U
t
T
I 1
/*-
D 2
 * Copyright (c) 1992 The Regents of the University of California.
 * All rights reserved.
E 2
I 2
 * Copyright (c) 1992, 1993
 *	The Regents of the University of California.  All rights reserved.
E 2
 *
 * %sccs.include.redist.c%
 */

#if defined(LIBC_SCCS) && !defined(lint)
	.asciz "%W% (Berkeley) %G%"
#endif /* LIBC_SCCS and not lint */

#include "DEFS.h"

ENTRY(__fixdfsi, 0)
	ldd	4(fp)
	cvdl	r0
	ret#1
E 1
