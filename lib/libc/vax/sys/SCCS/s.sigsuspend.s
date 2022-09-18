h65318
s 00002/00002/00020
d D 8.1 93/06/04 15:41:33 bostic 4 3
c 4.4BSD snapshot (revision 8.1); add 1993 to copyright
e
s 00001/00011/00021
d D 5.3 90/06/01 15:11:42 bostic 3 2
c new copyright notice
e
s 00001/00001/00031
d D 5.2 89/10/10 18:52:54 torek 2 1
c arguments are off ap, not fp
e
s 00032/00000/00000
d D 5.1 89/08/27 15:54:21 karels 1 0
c new call for posix
e
u
U
t
T
I 1
/*
D 4
 * Copyright (c) 1989 Regents of the University of California.
 * All rights reserved.
E 4
I 4
 * Copyright (c) 1989, 1993
 *	The Regents of the University of California.  All rights reserved.
E 4
 *
D 3
 * Redistribution and use in source and binary forms are permitted
 * provided that the above copyright notice and this paragraph are
 * duplicated in all such forms and that any documentation,
 * advertising materials, and other materials related to such
 * distribution and use acknowledge that the software was developed
 * by the University of California, Berkeley.  The name of the
 * University may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
E 3
I 3
 * %sccs.include.redist.c%
E 3
 */

#if defined(SYSLIBC_SCCS) && !defined(lint)
	.asciz "%W% (Berkeley) %G%"
#endif /* SYSLIBC_SCCS and not lint */

#include "SYS.h"

err:
	jmp	cerror

ENTRY(sigsuspend)
D 2
	movl	*4(fp),4(fp)		# indirect to mask arg
E 2
I 2
	movl	*4(ap),4(ap)		# indirect to mask arg
E 2
	chmk	$SYS_sigsuspend
	jcs	err
	clrl	r0			# shouldn't happen
	ret
E 1
