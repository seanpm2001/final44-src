h28639
s 00002/00002/00108
d D 8.1 93/06/11 16:21:27 bostic 17 16
c 4.4BSD snapshot (revision 8.1); add 1993 to copyright
e
s 00000/00004/00110
d D 7.5 91/11/01 16:53:23 bostic 16 15
c normalize KERNEL includes
e
s 00001/00011/00113
d D 7.4 90/06/28 22:31:05 bostic 15 14
c new copyright notice
e
s 00013/00002/00111
d D 7.3 89/04/24 18:50:35 mckusick 14 13
c add Berkeley copyright
e
s 00004/00000/00109
d D 7.2 87/04/13 11:11:23 bostic 13 12
c bug report 4.2BSD/sys/45
e
s 00001/00001/00108
d D 7.1 86/06/05 00:15:21 mckusick 12 11
c 4.3BSD release version
e
s 00007/00001/00102
d D 6.3 85/06/08 14:57:13 mckusick 11 10
c Add copyright
e
s 00001/00001/00102
d D 6.2 84/08/29 20:27:30 bloom 10 9
c Change to includes.  no more ../h
e
s 00000/00000/00103
d D 6.1 83/07/29 06:45:51 sam 9 8
c 4.2 distribution
e
s 00006/00007/00097
d D 2.4 83/05/27 12:50:08 sam 8 7
c merge of bill's code plus lint (plus, of course, cleanup of bill's bugs)
e
s 00000/00052/00104
d D 2.3 82/07/15 20:49:37 root 7 6
c get rid of tty stuff
e
s 00001/00003/00155
d D 2.2 82/04/19 19:36:27 mckusic 6 5
c merge into new file system
e
s 00000/00000/00158
d D 2.1 82/03/25 01:39:44 mckusick 5 4
c merge into the kernel!
e
s 00034/00104/00124
d D 1.4 82/03/23 12:57:05 mckusick 4 3
c compress frag tables
e
s 00085/00009/00143
d D 1.3 82/01/05 01:29:05 mckusic 3 2
c parameterize BSIZE, FSIZE, and FRAG into fs_bsize, fs_fsize, and fs_frag
e
s 00003/00003/00149
d D 1.2 81/10/07 18:31:52 mckusic 2 1
c fix unsigned char problem to get FRAG == 8 to work
e
s 00152/00000/00000
d D 1.1 81/10/04 16:23:24 mckusick 1 0
c date and time created 81/10/04 16:23:24 by mckusick
e
u
U
t
T
I 1
D 6
/* Copyright (c) 1981 Regents of the University of California */

/* %Z%%M% %I% %G% */
E 6
I 6
D 11
/*	%M%	%I%	%E%	*/
E 11
I 11
/*
D 12
 * Copyright (c) 1982 Regents of the University of California.
E 12
I 12
D 17
 * Copyright (c) 1982, 1986 Regents of the University of California.
E 12
D 14
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
E 14
I 14
 * All rights reserved.
E 17
I 17
 * Copyright (c) 1982, 1986, 1993
 *	The Regents of the University of California.  All rights reserved.
E 17
 *
D 15
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
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
E 15
I 15
 * %sccs.include.redist.c%
E 15
E 14
 *
 *	%W% (Berkeley) %G%
 */
E 11
E 6

I 13
D 16
#ifdef KERNEL
E 13
D 10
#include "../h/param.h"
E 10
I 10
#include "param.h"
I 13
#else
E 16
#include <sys/param.h>
D 16
#endif
E 16
E 13
E 10

D 7
/*	partab.c	4.2	81/03/08	*/

/*
 * Table giving parity for characters and indicating
 * character classes to tty driver.  In particular,
 * if the low 6 bits are 0, then the character needs
 * no special processing on output.
 */

D 2
char partab[] = {
E 2
I 2
unsigned char partab[] = {
E 2
	0001,0201,0201,0001,0201,0001,0001,0201,
	0202,0004,0003,0201,0005,0206,0201,0001,
	0201,0001,0001,0201,0001,0201,0201,0001,
	0001,0201,0201,0001,0201,0001,0001,0201,
	0200,0000,0000,0200,0000,0200,0200,0000,
	0000,0200,0200,0000,0200,0000,0000,0200,
	0000,0200,0200,0000,0200,0000,0000,0200,
	0200,0000,0000,0200,0000,0200,0200,0000,
	0200,0000,0000,0200,0000,0200,0200,0000,
	0000,0200,0200,0000,0200,0000,0000,0200,
	0000,0200,0200,0000,0200,0000,0000,0200,
	0200,0000,0000,0200,0000,0200,0200,0000,
	0000,0200,0200,0000,0200,0000,0000,0200,
	0200,0000,0000,0200,0000,0200,0200,0000,
	0200,0000,0000,0200,0000,0200,0200,0000,
	0000,0200,0200,0000,0200,0000,0000,0201,

	/*
	 * 7 bit ascii ends with the last character above,
	 * but we contine through all 256 codes for the sake
	 * of the tty output routines which use special vax
	 * instructions which need a 256 character trt table.
	 */

	0007,0007,0007,0007,0007,0007,0007,0007,
	0007,0007,0007,0007,0007,0007,0007,0007,
	0007,0007,0007,0007,0007,0007,0007,0007,
	0007,0007,0007,0007,0007,0007,0007,0007,
	0007,0007,0007,0007,0007,0007,0007,0007,
	0007,0007,0007,0007,0007,0007,0007,0007,
	0007,0007,0007,0007,0007,0007,0007,0007,
	0007,0007,0007,0007,0007,0007,0007,0007,
	0007,0007,0007,0007,0007,0007,0007,0007,
	0007,0007,0007,0007,0007,0007,0007,0007,
	0007,0007,0007,0007,0007,0007,0007,0007,
	0007,0007,0007,0007,0007,0007,0007,0007,
	0007,0007,0007,0007,0007,0007,0007,0007,
	0007,0007,0007,0007,0007,0007,0007,0007,
	0007,0007,0007,0007,0007,0007,0007,0007,
	0007,0007,0007,0007,0007,0007,0007,0007
};

E 7
/*
D 8
 * bit patterns for identifying fragments in the block map
E 8
I 8
 * Bit patterns for identifying fragments in the block map
E 8
 * used as ((map & around) == inside)
 */
int around[9] = {
	0x3, 0x7, 0xf, 0x1f, 0x3f, 0x7f, 0xff, 0x1ff, 0x3ff
};
int inside[9] = {
	0x0, 0x2, 0x6, 0xe, 0x1e, 0x3e, 0x7e, 0xfe, 0x1fe
};

/*
D 3
 * given a block map bit pattern, this table tells whether a
E 3
I 3
D 8
 * given a block map bit pattern, the frag tables tell whether a
E 8
I 8
 * Given a block map bit pattern, the frag tables tell whether a
E 8
E 3
 * particular size fragment is available. 
I 3
 *
E 3
 * used as:
D 3
 * if ((1 << (size - 1)) & fragtbl[map]) {
E 3
I 3
 * if ((1 << (size - 1)) & fragtbl[fs->fs_frag][map] {
E 3
 *	at least one fragment of the indicated size is available
 * }
D 3
 * This table is used by the scanc instruction on the VAX to
E 3
I 3
 *
 * These tables are used by the scanc instruction on the VAX to
E 3
 * quickly find an appropriate fragment.
 */
D 3
#if FRAG == 4
D 2
char fragtbl[256] = {
E 2
I 2
unsigned char fragtbl[256] = {
E 3
I 3
D 8

D 4
unsigned char fragtbl1[256] = {
	0x0, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1,
	0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1,
	0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1,
	0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1,
	0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1,
	0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1,
	0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1,
	0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1,
	0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1,
	0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1,
	0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1,
	0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1,
	0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1,
	0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1,
	0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1,
	0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1,
	0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1,
	0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1,
	0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1,
	0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1,
	0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1,
	0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1,
	0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1,
	0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1,
	0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1,
	0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1,
	0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1,
	0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1,
	0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1,
	0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1,
	0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1,
	0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1,
E 4
I 4
unsigned char fragtbl124[256] = {
E 8
I 8
u_char fragtbl124[256] = {
E 8
	0x00, 0x16, 0x16, 0x2a, 0x16, 0x16, 0x26, 0x4e,
	0x16, 0x16, 0x16, 0x3e, 0x2a, 0x3e, 0x4e, 0x8a,
	0x16, 0x16, 0x16, 0x3e, 0x16, 0x16, 0x36, 0x5e,
	0x16, 0x16, 0x16, 0x3e, 0x3e, 0x3e, 0x5e, 0x9e,
	0x16, 0x16, 0x16, 0x3e, 0x16, 0x16, 0x36, 0x5e,
	0x16, 0x16, 0x16, 0x3e, 0x3e, 0x3e, 0x5e, 0x9e,
	0x2a, 0x3e, 0x3e, 0x2a, 0x3e, 0x3e, 0x2e, 0x6e,
	0x3e, 0x3e, 0x3e, 0x3e, 0x2a, 0x3e, 0x6e, 0xaa,
	0x16, 0x16, 0x16, 0x3e, 0x16, 0x16, 0x36, 0x5e,
	0x16, 0x16, 0x16, 0x3e, 0x3e, 0x3e, 0x5e, 0x9e,
	0x16, 0x16, 0x16, 0x3e, 0x16, 0x16, 0x36, 0x5e,
	0x16, 0x16, 0x16, 0x3e, 0x3e, 0x3e, 0x5e, 0x9e,
	0x26, 0x36, 0x36, 0x2e, 0x36, 0x36, 0x26, 0x6e,
	0x36, 0x36, 0x36, 0x3e, 0x2e, 0x3e, 0x6e, 0xae,
	0x4e, 0x5e, 0x5e, 0x6e, 0x5e, 0x5e, 0x6e, 0x4e,
	0x5e, 0x5e, 0x5e, 0x7e, 0x6e, 0x7e, 0x4e, 0xce,
	0x16, 0x16, 0x16, 0x3e, 0x16, 0x16, 0x36, 0x5e,
	0x16, 0x16, 0x16, 0x3e, 0x3e, 0x3e, 0x5e, 0x9e,
	0x16, 0x16, 0x16, 0x3e, 0x16, 0x16, 0x36, 0x5e,
	0x16, 0x16, 0x16, 0x3e, 0x3e, 0x3e, 0x5e, 0x9e,
	0x16, 0x16, 0x16, 0x3e, 0x16, 0x16, 0x36, 0x5e,
	0x16, 0x16, 0x16, 0x3e, 0x3e, 0x3e, 0x5e, 0x9e,
	0x3e, 0x3e, 0x3e, 0x3e, 0x3e, 0x3e, 0x3e, 0x7e,
	0x3e, 0x3e, 0x3e, 0x3e, 0x3e, 0x3e, 0x7e, 0xbe,
	0x2a, 0x3e, 0x3e, 0x2a, 0x3e, 0x3e, 0x2e, 0x6e,
	0x3e, 0x3e, 0x3e, 0x3e, 0x2a, 0x3e, 0x6e, 0xaa,
	0x3e, 0x3e, 0x3e, 0x3e, 0x3e, 0x3e, 0x3e, 0x7e,
	0x3e, 0x3e, 0x3e, 0x3e, 0x3e, 0x3e, 0x7e, 0xbe,
	0x4e, 0x5e, 0x5e, 0x6e, 0x5e, 0x5e, 0x6e, 0x4e,
	0x5e, 0x5e, 0x5e, 0x7e, 0x6e, 0x7e, 0x4e, 0xce,
	0x8a, 0x9e, 0x9e, 0xaa, 0x9e, 0x9e, 0xae, 0xce,
	0x9e, 0x9e, 0x9e, 0xbe, 0xaa, 0xbe, 0xce, 0x8a,
E 4
};

D 4
unsigned char fragtbl2[256] = {
	0x0, 0x1, 0x1, 0x2, 0x1, 0x1, 0x1, 0x3,
	0x1, 0x1, 0x1, 0x3, 0x2, 0x3, 0x3, 0x2,
	0x1, 0x1, 0x1, 0x3, 0x1, 0x1, 0x1, 0x3,
	0x1, 0x1, 0x1, 0x3, 0x3, 0x3, 0x3, 0x3,
	0x1, 0x1, 0x1, 0x3, 0x1, 0x1, 0x1, 0x3,
	0x1, 0x1, 0x1, 0x3, 0x3, 0x3, 0x3, 0x3,
	0x2, 0x3, 0x3, 0x2, 0x3, 0x3, 0x3, 0x3,
	0x3, 0x3, 0x3, 0x3, 0x2, 0x3, 0x3, 0x2,
	0x1, 0x1, 0x1, 0x3, 0x1, 0x1, 0x1, 0x3,
	0x1, 0x1, 0x1, 0x3, 0x3, 0x3, 0x3, 0x3,
	0x1, 0x1, 0x1, 0x3, 0x1, 0x1, 0x1, 0x3,
	0x1, 0x1, 0x1, 0x3, 0x3, 0x3, 0x3, 0x3,
	0x1, 0x1, 0x1, 0x3, 0x1, 0x1, 0x1, 0x3,
	0x1, 0x1, 0x1, 0x3, 0x3, 0x3, 0x3, 0x3,
	0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3,
	0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3,
	0x1, 0x1, 0x1, 0x3, 0x1, 0x1, 0x1, 0x3,
	0x1, 0x1, 0x1, 0x3, 0x3, 0x3, 0x3, 0x3,
	0x1, 0x1, 0x1, 0x3, 0x1, 0x1, 0x1, 0x3,
	0x1, 0x1, 0x1, 0x3, 0x3, 0x3, 0x3, 0x3,
	0x1, 0x1, 0x1, 0x3, 0x1, 0x1, 0x1, 0x3,
	0x1, 0x1, 0x1, 0x3, 0x3, 0x3, 0x3, 0x3,
	0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3,
	0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3,
	0x2, 0x3, 0x3, 0x2, 0x3, 0x3, 0x3, 0x3,
	0x3, 0x3, 0x3, 0x3, 0x2, 0x3, 0x3, 0x2,
	0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3,
	0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3,
	0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3,
	0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3,
	0x2, 0x3, 0x3, 0x2, 0x3, 0x3, 0x3, 0x3,
	0x3, 0x3, 0x3, 0x3, 0x2, 0x3, 0x3, 0x2,
};

unsigned char fragtbl4[256] = {
E 3
E 2
	0x0, 0x1, 0x1, 0x2, 0x1, 0x1, 0x2, 0x4,
	0x1, 0x1, 0x1, 0x3, 0x2, 0x3, 0x4, 0x8,
	0x1, 0x1, 0x1, 0x3, 0x1, 0x1, 0x3, 0x5,
	0x1, 0x1, 0x1, 0x3, 0x3, 0x3, 0x5, 0x9,
	0x1, 0x1, 0x1, 0x3, 0x1, 0x1, 0x3, 0x5,
	0x1, 0x1, 0x1, 0x3, 0x3, 0x3, 0x5, 0x9,
	0x2, 0x3, 0x3, 0x2, 0x3, 0x3, 0x2, 0x6,
	0x3, 0x3, 0x3, 0x3, 0x2, 0x3, 0x6, 0xa,
	0x1, 0x1, 0x1, 0x3, 0x1, 0x1, 0x3, 0x5,
	0x1, 0x1, 0x1, 0x3, 0x3, 0x3, 0x5, 0x9,
	0x1, 0x1, 0x1, 0x3, 0x1, 0x1, 0x3, 0x5,
	0x1, 0x1, 0x1, 0x3, 0x3, 0x3, 0x5, 0x9,
	0x2, 0x3, 0x3, 0x2, 0x3, 0x3, 0x2, 0x6,
	0x3, 0x3, 0x3, 0x3, 0x2, 0x3, 0x6, 0xa,
	0x4, 0x5, 0x5, 0x6, 0x5, 0x5, 0x6, 0x4,
	0x5, 0x5, 0x5, 0x7, 0x6, 0x7, 0x4, 0xc,
	0x1, 0x1, 0x1, 0x3, 0x1, 0x1, 0x3, 0x5,
	0x1, 0x1, 0x1, 0x3, 0x3, 0x3, 0x5, 0x9,
	0x1, 0x1, 0x1, 0x3, 0x1, 0x1, 0x3, 0x5,
	0x1, 0x1, 0x1, 0x3, 0x3, 0x3, 0x5, 0x9,
	0x1, 0x1, 0x1, 0x3, 0x1, 0x1, 0x3, 0x5,
	0x1, 0x1, 0x1, 0x3, 0x3, 0x3, 0x5, 0x9,
	0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x7,
	0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x7, 0xb,
	0x2, 0x3, 0x3, 0x2, 0x3, 0x3, 0x2, 0x6,
	0x3, 0x3, 0x3, 0x3, 0x2, 0x3, 0x6, 0xa,
	0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x7,
	0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x7, 0xb,
	0x4, 0x5, 0x5, 0x6, 0x5, 0x5, 0x6, 0x4,
	0x5, 0x5, 0x5, 0x7, 0x6, 0x7, 0x4, 0xc,
	0x8, 0x9, 0x9, 0xa, 0x9, 0x9, 0xa, 0xc,
	0x9, 0x9, 0x9, 0xb, 0xa, 0xb, 0xc, 0x8,
};
D 3
#endif
E 3

E 4
D 3
#if FRAG == 8
D 2
char fragtbl[256] = {
E 2
I 2
unsigned char fragtbl[256] = {
E 3
I 3
D 8
unsigned char fragtbl8[256] = {
E 8
I 8
u_char fragtbl8[256] = {
E 8
E 3
E 2
	0x00, 0x01, 0x01, 0x02, 0x01, 0x01, 0x02, 0x04,
	0x01, 0x01, 0x01, 0x03, 0x02, 0x03, 0x04, 0x08,
	0x01, 0x01, 0x01, 0x03, 0x01, 0x01, 0x03, 0x05,
	0x02, 0x03, 0x03, 0x02, 0x04, 0x05, 0x08, 0x10,
	0x01, 0x01, 0x01, 0x03, 0x01, 0x01, 0x03, 0x05,
	0x01, 0x01, 0x01, 0x03, 0x03, 0x03, 0x05, 0x09,
	0x02, 0x03, 0x03, 0x02, 0x03, 0x03, 0x02, 0x06,
	0x04, 0x05, 0x05, 0x06, 0x08, 0x09, 0x10, 0x20,
	0x01, 0x01, 0x01, 0x03, 0x01, 0x01, 0x03, 0x05,
	0x01, 0x01, 0x01, 0x03, 0x03, 0x03, 0x05, 0x09,
	0x01, 0x01, 0x01, 0x03, 0x01, 0x01, 0x03, 0x05,
	0x03, 0x03, 0x03, 0x03, 0x05, 0x05, 0x09, 0x11,
	0x02, 0x03, 0x03, 0x02, 0x03, 0x03, 0x02, 0x06,
	0x03, 0x03, 0x03, 0x03, 0x02, 0x03, 0x06, 0x0a,
	0x04, 0x05, 0x05, 0x06, 0x05, 0x05, 0x06, 0x04,
	0x08, 0x09, 0x09, 0x0a, 0x10, 0x11, 0x20, 0x40,
	0x01, 0x01, 0x01, 0x03, 0x01, 0x01, 0x03, 0x05,
	0x01, 0x01, 0x01, 0x03, 0x03, 0x03, 0x05, 0x09,
	0x01, 0x01, 0x01, 0x03, 0x01, 0x01, 0x03, 0x05,
	0x03, 0x03, 0x03, 0x03, 0x05, 0x05, 0x09, 0x11,
	0x01, 0x01, 0x01, 0x03, 0x01, 0x01, 0x03, 0x05,
	0x01, 0x01, 0x01, 0x03, 0x03, 0x03, 0x05, 0x09,
	0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x07,
	0x05, 0x05, 0x05, 0x07, 0x09, 0x09, 0x11, 0x21,
	0x02, 0x03, 0x03, 0x02, 0x03, 0x03, 0x02, 0x06,
	0x03, 0x03, 0x03, 0x03, 0x02, 0x03, 0x06, 0x0a,
	0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x07,
	0x02, 0x03, 0x03, 0x02, 0x06, 0x07, 0x0a, 0x12,
	0x04, 0x05, 0x05, 0x06, 0x05, 0x05, 0x06, 0x04,
	0x05, 0x05, 0x05, 0x07, 0x06, 0x07, 0x04, 0x0c,
	0x08, 0x09, 0x09, 0x0a, 0x09, 0x09, 0x0a, 0x0c,
	0x10, 0x11, 0x11, 0x12, 0x20, 0x21, 0x40, 0x80,
};
D 3
#endif
E 3
I 3

/*
D 8
 * the actual fragtbl array
E 8
I 8
 * The actual fragtbl array.
E 8
 */
D 8
unsigned char *fragtbl[MAXFRAG + 1] = {
E 8
I 8
u_char *fragtbl[MAXFRAG + 1] = {
E 8
D 4
	0, fragtbl1, fragtbl2, 0, fragtbl4, 0, 0, 0, fragtbl8,
E 4
I 4
	0, fragtbl124, fragtbl124, 0, fragtbl124, 0, 0, 0, fragtbl8,
E 4
};
E 3
E 1