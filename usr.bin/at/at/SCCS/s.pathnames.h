h07806
s 00002/00002/00010
d D 8.1 93/06/06 14:20:59 bostic 5 4
c 4.4BSD snapshot (revision 8.1); add 1993 to copyright
e
s 00001/00011/00011
d D 5.4 90/06/01 16:35:46 bostic 4 3
c new copyright notice
e
s 00000/00003/00022
d D 5.3 90/04/19 13:49:01 bostic 3 2
c break up at directories
e
s 00003/00003/00022
d D 5.2 89/07/28 12:58:34 bostic 2 1
c /var/spool/at -> /var/at
e
s 00025/00000/00000
d D 5.1 89/05/11 14:41:14 bostic 1 0
c file reorg, pathnames.h, paths.h
e
u
U
t
T
I 1
/*
D 5
 * Copyright (c) 1989 The Regents of the University of California.
 * All rights reserved.
E 5
I 5
 * Copyright (c) 1989, 1993
 *	The Regents of the University of California.  All rights reserved.
E 5
 *
D 4
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
E 4
I 4
 * %sccs.include.redist.c%
E 4
 *
 *	%W% (Berkeley) %G%
 */

#include <paths.h>

D 2
#define	_PATH_ATDIR	"/var/spool/at"
#define	_PATH_LASTFILE	"/var/spool/at/lasttimedone"
E 2
I 2
#define	_PATH_ATDIR	"/var/at"
D 3
#define	_PATH_LASTFILE	"/var/at/lasttimedone"
E 2
#define	_PATH_MAIL	"/bin/mail"
D 2
#define	_PATH_PAST	"/var/spool/at/past"
E 2
I 2
#define	_PATH_PAST	"/var/at/past"
E 3
E 2
E 1
