h16511
s 00002/00002/00029
d D 8.1 93/06/04 18:19:24 bostic 4 3
c 4.4BSD snapshot (revision 8.1); add 1993 to copyright
e
s 00008/00001/00023
d D 4.3 91/04/22 16:09:33 bostic 3 2
c new copyright; att/bsd/shared
e
s 00007/00007/00017
d D 4.2 83/11/10 15:22:50 ralph 2 1
c fixed warning messages.
e
s 00024/00000/00000
d D 4.1 83/06/27 14:26:19 sam 1 0
c date and time created 83/06/27 14:26:19 by sam
e
u
U
t
T
I 3
/*-
D 4
 * Copyright (c) 1983 The Regents of the University of California.
 * All rights reserved.
E 4
I 4
 * Copyright (c) 1983, 1993
 *	The Regents of the University of California.  All rights reserved.
E 4
 *
 * %sccs.include.proprietary.c%
 */

E 3
I 1
#ifndef lint
static char sccsid[] = "%W% (Berkeley) %G%";
D 3
#endif
E 3
I 3
#endif /* not lint */
E 3

int xnow;
int ynow;
D 2
float boty 0.;
float botx 0.;
float oboty 0.;
float obotx 0.;
float scalex 1.;
float scaley 1.;
int vti -1;
E 2
I 2
float boty = 0.;
float botx = 0.;
float oboty = 0.;
float obotx = 0.;
float scalex = 1.;
float scaley = 1.;
int vti = -1;
E 2

openvt ()
{
		vti = open("/dev/vt0",1);
		return;
}
openpl()
{
	vti = open("/dev/vt0",1);
	return;
}
E 1
