h47041
s 00002/00002/00014
d D 8.1 93/06/04 15:44:27 bostic 2 1
c 4.4BSD snapshot (revision 8.1); add 1993 to copyright
e
s 00016/00000/00000
d D 5.1 92/06/25 09:11:45 bostic 1 0
c date and time created 92/06/25 09:11:45 by bostic
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

ENTRY(__lshlsi3, 0)
	ashl	8(ap),4(ap),r0
	ret
E 1
