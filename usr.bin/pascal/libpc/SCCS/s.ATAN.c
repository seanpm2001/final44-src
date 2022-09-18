h20811
s 00002/00002/00025
d D 8.1 93/06/06 15:14:42 bostic 4 3
c 4.4BSD snapshot (revision 8.1); add 1993 to copyright
e
s 00009/00002/00018
d D 1.3 90/04/09 18:25:59 bostic 3 2
c written by Kirk McKusick; add Berkeley specific copyright notice
e
s 00000/00001/00020
d D 1.2 89/01/09 15:38:23 mckusick 2 1
c update from John Gilmore for gcc
e
s 00021/00000/00000
d D 1.1 82/02/08 20:32:58 mckusick 1 0
c date and time created 82/02/08 20:32:58 by mckusick
e
u
U
t
T
I 1
D 3
/* Copyright (c) 1982 Regents of the University of California */
E 3
I 3
/*-
D 4
 * Copyright (c) 1982 The Regents of the University of California.
 * All rights reserved.
E 4
I 4
 * Copyright (c) 1982, 1993
 *	The Regents of the University of California.  All rights reserved.
E 4
 *
 * %sccs.include.redist.c%
 */
E 3

D 3
static char sccsid[] = "%Z%%M% %I% %G%";
E 3
I 3
#ifndef lint
static char sccsid[] = "%W% (Berkeley) %G%";
#endif /* not lint */
E 3

#include <math.h>
extern int errno;

double
ATAN(value)
	double	value;
{
	double result;

	errno = 0;
	result = atan(value);
	if (errno != 0) {
		ERROR("Argument %e is out of the domain of atan\n", value);
D 2
		return;
E 2
	}
	return result;
}
E 1
