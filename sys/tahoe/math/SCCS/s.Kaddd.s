h06629
s 00011/00001/00162
d D 7.1 90/12/06 13:49:35 bostic 5 4
c add Berkeley specific copyright notice
e
s 00001/00001/00162
d D 1.4 90/12/04 22:51:06 bostic 4 3
c redo include files for kernel reorg
e
s 00002/00002/00161
d D 1.3 86/01/05 18:50:35 sam 3 2
c working (mostly)
e
s 00044/00045/00119
d D 1.2 86/01/03 23:46:33 sam 2 1
c massive cleanups; use SYS.h macros to allow profiling; moved to 
c tahoemath directory; revamp comments for cpp
e
s 00164/00000/00000
d D 1.1 85/08/02 15:42:20 sam 1 0
c date and time created 85/08/02 15:42:20 by sam
e
u
U
t
T
I 2
D 5
/*	%M%	%I%	%E%	*/
E 5
I 5
/*-
 * Copyright (c) 1985 The Regents of the University of California.
 * All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Computer Consoles Inc.
 *
 * %sccs.include.redist.c%
 *
 *	%W% (Berkeley) %G%
 */
E 5
E 2
I 1

D 2
#include	"fp.h"
E 2
I 2
D 3
#include "fp.h"
#include "SYS.h"
E 3
I 3
D 4
#include "../tahoemath/fp.h"
E 4
I 4
#include "../math/fp.h"
E 4
#include "../tahoe/SYS.h"
E 3
E 2

I 2
/*
 * _Kaddd(acc_most,acc_least,op_most,op_least,hfs)
 */
E 2

D 2

	.text
	.globl	_Kaddd	# _Kaddd(acc_most,acc_least,op_most,op_least,hfs)
_Kaddd:	.word	0x1ffc	# we use many registers...

  #
  # see which operand has a greater exponent
  # The greater one will be fetched into r0,r1,r2,r3.
  # r0,r1 - 'pure' fraction, r2 - exponent, r3 - sign).
  # The smaller operand will be fetched into r4,r5,r6,r7.
  #
E 2
I 2
ENTRY(Kaddd, R10|R9|R8|R7|R6|R5|R4|R3|R2)
/*
 * see which operand has a greater exponent
 * The greater one will be fetched into r0,r1,r2,r3.
 * r0,r1 - 'pure' fraction, r2 - exponent, r3 - sign).
 * The smaller operand will be fetched into r4,r5,r6,r7.
 */
E 2
	tstl	4(fp)	# handle (a+b) where a and/or b = 0.0
	jneq	next
	movl	16(fp),r1
	movl	12(fp),r0
	ret
next:
	tstl	12(fp)
	jneq	doit
	movl	8(fp),r1
	movl	4(fp),r0
	ret
doit:
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


D 2
 #
 #compute exponents:
 #
E 2
I 2
/*
 * Compute exponents:
 */
E 2
expo:
	andl3	$EXPMASK,r0,r2	# r2 will hold the exponent.
	shrl	$EXPSHIFT,r2,r2
	andl3	$EXPMASK,r4,r6	# r6 will hold the exponent.
	shrl	$EXPSHIFT,r6,r6
D 2
 #
 #compare the exponents:
 #
E 2
I 2
/*
 * Compare the exponents:
 */
E 2
	subl3	r6,r2,r8
	jeql	signs
	cmpl	r8,$MAX_EXP_DIF
	jleq	signs
	ret			# return the bigger number.
 
D 2
 #
 #remember the signs:
 #
E 2
I 2
/*
 * Remember the signs:
 */
E 2
signs:
	clrl	r3
	bbc	$31,r0,sign2	# if negative remember it.
	incl	r3
sign2:
	clrl	r7
	bbc	$31,r4,frac	# if negative remember it.
	incl	r7
D 2
 #
 #compute 'pure' fraction:
 #
E 2
I 2
/*
 * Compute 'pure' fraction:
 */
E 2
frac:
				# clear the non fraction parts.
	andl2	$(0!(EXPMASK | SIGNBIT)),r0
				# add the hidden bit.
	orl2	$(0!CLEARHID),r0
				# clear the non fraction parts.
	andl2	$(0!(EXPMASK | SIGNBIT)),r4
				# add the hidden bit.
	orl2	$(0!CLEARHID),r4

D 2
 #
 #shift the smaller operand:
 #
E 2
I 2
/*
 * Shift the smaller operand:
 */
E 2
	shrq	r8,r4,r4
eql_exps:
	cmpl 	r3,r7
	jeql	add
	bbc	$0,r3,negr4r5
D 2
 #
 #negate the pair r0,r1:
 #
E 2
I 2
/*
 * Negate the pair r0,r1:
 */
E 2
	clrl	r3
	mcoml	r1,r1
	clrl	r9		# r9 - carry flag.
	incl	r1
	bcc	comr0
	incl	r9		# remember the carry.
comr0:	mcoml	r0,r0
	bbc	$0,r9,add
	incl	r0

D 2
 #
 #add the fractions:
 #
E 2
I 2
/*
 * Add the fractions:
 */
E 2
add:
	clrl	r10 		# to remember the sign of the result.
	addl2	r5,r1
	adwc	r4,r0
	jgeq	norm		# if positive go to normelize.
	incl	r10		# else remember it and negate the result.
D 2
 #
 #negate the pair r0,r1:
 #
E 2
I 2
/*
 * Negate the pair r0,r1:
 */
E 2
	clrl	r3
	mcoml	r1,r1
	clrl	r9		# r9 - carry flag.
	incl	r1
	bcc	comr00
	incl	r9		# remember the carry.
comr00:	mcoml	r0,r0
	bbc	$0,r9,norm
	incl	r0
norm:	pushl	20(fp)		# addr of returnen exception.
D 2
	callf	$8,Kfnorm
E 2
I 2
	callf	$8,_Kfnorm
E 2
 
D 2
 #
 #add the sign bit
 #
E 2
I 2
/*
 * Add the sign bit
 */
E 2
	bbs	$0,r10,negative
	bbs	$0,r3,negative
	ret
negative:
	orl2	$SIGNBIT,r0
	ret
 
 
D 2
 #
 #negate the pair r4,r5:
 #
E 2
I 2
/*
 * Negate the pair r4,r5:
 */
E 2
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
 
D 2
 
E 2
	movl	r4,r0		# return the  smaller operand.
	movl	r5,r1
	ret
D 2

E 2
E 1
