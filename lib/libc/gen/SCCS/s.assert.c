h03055
s 00002/00002/00025
d D 8.1 93/06/04 12:00:16 bostic 2 1
c 4.4BSD snapshot (revision 8.1); add 1993 to copyright
e
s 00027/00000/00000
d D 5.1 92/04/23 15:55:23 bostic 1 0
c date and time created 92/04/23 15:55:23 by bostic
e
u
U
t
T
I 1
/*-
D 2
 * Copyright (c) 1992 The Regents of the University of California.
 * All rights reserved.
E 2
I 2
 * Copyright (c) 1992, 1993
 *	The Regents of the University of California.  All rights reserved.
E 2
 *
 * %sccs.include.redist.c%
 */

#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "%W% (Berkeley) %G%";
#endif /* LIBC_SCCS and not lint */

#include <sys/types.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void
__assert(file, line, failedexpr)
	const char *file, *failedexpr;
	int line;
{
	(void)fprintf(stderr,
	    "assertion \"%s\" failed: file \"%s\", line %d\n",
	    failedexpr, file, line);
	abort();
	/* NOTREACHED */
}
E 1
