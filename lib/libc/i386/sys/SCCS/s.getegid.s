h40904
s 00019/00000/00000
d D 5.1 90/04/23 19:38:55 william 1 0
c date and time created 90/04/23 19:38:55 by william
e
u
U
t
T
I 1
/*-
 * Copyright (c) 1990 The Regents of the University of California.
 * All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * William Jolitz.
 *
 * %sccs.include.redist.c%
 */

#if defined(SYSLIBC_SCCS) && !defined(lint)
	.asciz "%W% (Berkeley) %G%"
#endif /* SYSLIBC_SCCS and not lint */

#include "SYS.h"

PSEUDO(getegid,getgid)
	movl	%edx,%eax
	ret			/* egid = getegid(); */
E 1
