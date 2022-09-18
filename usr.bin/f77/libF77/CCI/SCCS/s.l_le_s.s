h23125
s 00013/00001/00037
d D 5.2 91/04/12 15:15:59 bostic 2 1
c new copyright; att/bsd/shared
e
s 00038/00000/00000
d D 5.1 86/11/03 20:00:45 mckusick 1 0
c from CCI
e
u
U
t
T
I 1
D 2
#	"%W% (Berkeley) %G%"
E 2
I 2
/*-
 * Copyright (c) 1991 The Regents of the University of California.
 * All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Computer Consoles Inc.
 *
 * %sccs.include.proprietary.c%
 */

#ifndef lint
	.asciz "%W% (Berkeley) %G%"
#endif /* not lint */
E 2

.data
.text
LL0:.align	1
.globl	_l_le
.set	MASK__,0x4
.data
.text
_l_le:.word	MASK__
	movl	4(fp),r0		/* a */
	movl	8(fp),r1		/* b */
	cmpl	12(fp),16(fp)		/* if (la <= lb) */
	jgtr	LB
	movl	12(fp), r2		/* compare according to la */
	cmps3
	jleq	out1			/* if less or equal return(1) */
	jbr	out0			/* else greater: return(0) */

LB:					/* else */
	movl	16(fp), r2		/* compare according to lb */
	cmps3
	jlss	out1			/* if less return(1) */
	jgtr	out0			/* if greater return(0) */

	addl3	4(fp), 12(fp), r2	/* aend */
LOOP2:
	cmpb	(r0), $32		/* if *a != space */
	jneq	out0			/* then astring > bstring */
	incl	r0			/* else continue */
	cmpl	r0, r2			/* till aend */
	jlssu	LOOP2
out1:
	movl	$1, r0			/* else return(1) */
	ret
out0:
	clrl	r0
	ret
E 1
