h08989
s 00002/00002/00029
d D 8.1 93/06/04 13:24:30 bostic 3 2
c 4.4BSD snapshot (revision 8.1); add 1993 to copyright
e
s 00001/00011/00030
d D 5.2 90/06/01 15:35:01 bostic 2 1
c new copyright notice
e
s 00041/00000/00000
d D 5.1 89/08/27 15:58:48 karels 1 0
c new call for posix
e
u
U
t
T
I 1
/*
D 3
 * Copyright (c) 1989 Regents of the University of California.
 * All rights reserved.
E 3
I 3
 * Copyright (c) 1989, 1993
 *	The Regents of the University of California.  All rights reserved.
E 3
 *
D 2
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
E 2
I 2
 * %sccs.include.redist.c%
E 2
 */

#if defined(SYSLIBC_SCCS) && !defined(lint)
	.asciz "%W% (Berkeley) %G%"
#endif /* SYSLIBC_SCCS and not lint */

#include "SYS.h"

err:
	jmp	cerror

ENTRY(sigprocmask)
	tstl	8(fp)			# check new sigset pointer
	bneq	1f			# if not null, indirect
	movl	$0,8(fp)		# null mask pointer: block empty set
	movl	$1,4(fp)		# SIG_BLOCK
	jbr	2f
1:	movl	*8(fp),8(fp)		# indirect to new mask arg
2:	kcall	$SYS_sigprocmask
	jcs	err
	tstl	12(fp)			# test if old mask requested
	beql	out
	movl	r0,*12(fp)		# store old mask
out:
	clrl	r0
	ret
E 1
