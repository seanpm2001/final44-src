h05980
s 00002/00002/00170
d D 8.1 93/06/04 13:13:38 bostic 4 3
c 4.4BSD snapshot (revision 8.1); add 1993 to copyright
e
s 00001/00011/00171
d D 1.3 90/06/01 14:46:48 bostic 3 2
c new copyright notice
e
s 00023/00003/00159
d D 1.2 89/02/17 17:35:21 bostic 2 1
c add Berkeley specific copyright notice
e
s 00162/00000/00000
d D 1.1 86/07/02 16:25:45 sam 1 0
c date and time created 86/07/02 16:25:45 by sam
e
u
U
t
T
I 1
D 2
#ifdef LIBC_SCCS
	.asciz	"%W% (Berkeley/CCI) %G%"
#endif LIBC_SCCS
E 2
I 2
/*
D 4
 * Copyright (c) 1986 Regents of the University of California.
 * All rights reserved.
E 4
I 4
 * Copyright (c) 1986, 1993
 *	The Regents of the University of California.  All rights reserved.
E 4
 *
 * This code is derived from software contributed to Berkeley by
 * Computer Consoles Inc.
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
E 2

#include <tahoemath/fp.h>
#include "DEFS.h"

XENTRY(addd, R2|R3|R4|R5|R6|R7|R8|R9|R10)
/*
 * see which operand has a greater exponent
 * The greater one will be fetched into r0,r1,r2,r3.
 * r0,r1 - 'pure' fraction, r2 - exponent, r3 - sign).
 * The smaller operand will be fetched into r4,r5,r6,r7.
 */
	andl3	$EXPMASK,4(fp),r0
	andl3	$EXPMASK,12(fp),r1
	cmpl	r0,r1
	jgtr	first_greater

	movl	12(fp),r0	# bigger operand to r0,r1
	movl	16(fp),r1

	movl	4(fp),r4	# smaller operand to r4,r5
	movl	8(fp),r5
	jmp	expo

first_greater:
	movl	4(fp),r0	# bigger operand to r0,r1
	movl	8(fp),r1

	movl	12(fp),r4	# smaller operand to r4,r5
	movl	16(fp),r5

/*
 * compute exponents:
 */
expo:
	andl3	$EXPMASK,r0,r2	# r2 will hold the exponent.
	jeql	is_res1		# check for reserved operand. 
	shrl	$EXPSHIFT,r2,r2


	andl3	$EXPMASK,r4,r6	# r6 will hold the exponent.
	jeql	is_res2		# check for reserved operand. 
	shrl	$EXPSHIFT,r6,r6
/*
 * compare the exponents:
 */
	subl3	r6,r2,r8
	jeql	signs
	cmpl	r8,$MAX_EXP_DIF
	jlss	signs
	ret			# return the bigger number.
 
/*
 * remember the signs:
 */
signs:
	clrl	r3
	bbc	$31,r0,sign2	# if negative remember it.
	incl	r3
sign2:
	clrl	r7
	bbc	$31,r4,frac	# if negative remember it.
	incl	r7
/*
 * compute 'pure' fraction:
 */
frac:
				# clear the non fraction parts.
	andl2	$(0!(EXPMASK | SIGNBIT)),r0
				# add the hidden bit.
	orl2	$(0!CLEARHID),r0
				# clear the non fraction parts.
	andl2	$(0!(EXPMASK | SIGNBIT)),r4
				# add the hidden bit.
	orl2	$(0!CLEARHID),r4

/*
 * shift the smaller operand:
 */
	shrq	r8,r4,r4
eql_exps:
	cmpl 	r3,r7
	jeql	add
	bbc	$0,r3,negr4r5
/*
 * negate the pair r0,r1:
 */
	clrl	r3
	mcoml	r1,r1
	clrl	r9		# r9 - carry flag.
	incl	r1
	bcc	comr0
	incl	r9		# remember the carry.
comr0:	mcoml	r0,r0
	bbc	$0,r9,add
	incl	r0

/*
 * add the fractions:
 */
add:
	clrl	r10 		# to remember the sign of the result.
	addl2	r5,r1
	adwc	r4,r0
	jgeq	norm		# if positive go to normelize.
	incl	r10		# else remember it and negate the result.
/*
 * negate the pair r0,r1:
 */
	clrl	r3
	mcoml	r1,r1
	clrl	r9		# r9 - carry flag.
	incl	r1
	bcc	comr00
	incl	r9		# remember the carry.
comr00:	mcoml	r0,r0
	bbc	$0,r9,norm
	incl	r0
norm:	callf	$4,fnorm
 
/*
 * add the sign bit
 */
	bbs	$0,r10,negative
	bbs	$0,r3,negative
	ret
negative:
	orl2	$SIGNBIT,r0
	ret


/*
 * negate the pair r4,r5:
 */
negr4r5:
	clrl	r7
	mcoml	r5,r5
	clrl	r9		# r9 - carry flag.
	incl	r5
	bcc	comr4
	incl	r9		# remember the carry.
comr4:	mcoml	r4,r4
	bbc	$0,r9,add
	incl	r4
	jmp	add
 
 
is_res1:
	bbs	$31,r0,res_op
	movl	r4,r0		# return the  smaller operand.
	movl	r5,r1
	ret

is_res2:
	bbs	$31,r4,res_op
	ret			# we allready have the 'result' in r0,r1.

res_op:
	callf	$4,fpresop
	ret
E 1
