h14695
s 00002/00002/00026
d D 8.1 93/06/04 13:02:04 bostic 2 1
c 4.4BSD snapshot (revision 8.1); add 1993 to copyright
e
s 00028/00000/00000
d D 5.1 92/06/25 06:34:22 torek 1 0
c date and time created 92/06/25 06:34:22 by torek
e
u
U
t
T
I 1
/*
D 2
 * Copyright (c) 1992 The Regents of the University of California.
 * All rights reserved.
E 2
I 2
 * Copyright (c) 1992, 1993
 *	The Regents of the University of California.  All rights reserved.
E 2
 *
 * This software was developed by the Computer Systems Engineering group
 * at Lawrence Berkeley Laboratory under DARPA contract BG 91-66 and
 * contributed to Berkeley.
 *
 * %sccs.include.redist.c%
 *
 * from: $Header: strlen.s,v 1.1 92/06/25 12:52:47 torek Exp $
 */

#if defined(LIBC_SCCS) && !defined(lint)
	.asciz "%W% (Berkeley) %G%"
#endif /* LIBC_SCCS and not lint */

#include "DEFS.h"

ENTRY(strlen)
	add	%o0, 1, %o1	! save starting point + 1
1:
	ldsb	[%o0], %o2	! fetch byte
	tst	%o2		! null?
	bne	1b		! no, keep going
	inc	%o0		! always increment pointer
	retl
	sub	%o0, %o1, %o0	! return length (ptr - (origptr+1))
E 1
