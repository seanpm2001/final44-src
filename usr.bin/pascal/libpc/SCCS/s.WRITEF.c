h43223
s 00002/00002/00029
d D 8.1 93/06/06 15:20:33 bostic 7 6
c 4.4BSD snapshot (revision 8.1); add 1993 to copyright
e
s 00009/00002/00022
d D 1.5 90/04/09 18:28:09 bostic 6 5
c written by Kirk McKusick; add Berkeley specific copyright notice
e
s 00003/00003/00021
d D 1.4 81/06/10 00:45:05 mckusic 5 4
c move the text of error messages from ERROR to the functions
c which generate them.
e
s 00003/00035/00021
d D 1.3 81/03/10 00:41:11 mckusic 4 2
c onyx optimizations
e
s 00000/00000/00056
d R 1.3 81/03/10 00:30:38 mckusic 3 2
c onyx optimizations
e
s 00036/00002/00020
d D 1.2 81/03/07 16:06:11 mckusic 2 1
c merge in onyx changes
e
s 00022/00000/00000
d D 1.1 80/10/30 00:36:24 mckusick 1 0
c date and time created 80/10/30 00:36:24 by mckusick
e
u
U
t
T
I 1
D 6
/* Copyright (c) 1979 Regents of the University of California */
E 6
I 6
/*-
D 7
 * Copyright (c) 1979 The Regents of the University of California.
 * All rights reserved.
E 7
I 7
 * Copyright (c) 1979, 1993
 *	The Regents of the University of California.  All rights reserved.
E 7
 *
 * %sccs.include.redist.c%
 */
E 6

D 6
static char sccsid[] = "%Z%%M% %I% %G%";
E 6
I 6
#ifndef lint
static char sccsid[] = "%W% (Berkeley) %G%";
#endif /* not lint */
E 6

#include "h00vars.h"
D 5
#include "h01errs.h"
E 5

D 4
WRITEF(curfile, d1, d2, d3, d4, d5, d6)
E 4
I 4
WRITEF(curfile, d1, d2, d3, d4, d5, d6, d7, d8)
E 4

	register struct iorec	*curfile;
D 2
	int			d1, d2, d3, d4, d5, d6;
E 2
I 2
	FILE			*d1;
	char			*d2;
D 4
	long			d3, d4, d5, d6;
E 4
I 4
	int			d3, d4, d5, d6, d7, d8;
E 4
E 2
{
	if (curfile->funit & FREAD) {
D 5
		ERROR(EWRITEIT, curfile->pfname);
E 5
I 5
		ERROR("%s: Attempt to write, but open for reading\n",
			curfile->pfname);
E 5
		return;
	}
D 2
	fprintf(d1, d2, d3, d4, d5, d6);
E 2
I 2
D 4
#	ifdef VAX
		fprintf(d1, d2, d3, d4, d5, d6);
#	else
		{
		register char	*cp;
		short		scnt;

		scnt = 0;
		for (cp = (char *)d2; *cp; )
			switch(*cp++) {
			case '*':
			case 's':
			case 'c':
				scnt++;
			}
		switch(scnt) {
		case 0:
			fprintf(d1, d2, d3, d4);
			break;
		case 1:
			fprintf(d1, d2, (int)d3, d4, d5);
			break;
		case 2:
			fprintf(d1, d2, (int)d3, (int)d4, d5, d6);
			break;
		default:
			fprintf(stderr, "Panic: bad argcount %d to WRITEF\n",
				scnt);
			ERROR(EWRITE, curfile->pfname);
			return;
		}
	}
#	endif VAX
E 4
I 4
	fprintf(d1, d2, d3, d4, d5, d6, d7, d8);
E 4
E 2
	if (ferror(curfile->fbuf)) {
D 5
		ERROR(EWRITE, curfile->pfname);
E 5
I 5
		PERROR("Could not write to ", curfile->pfname);
E 5
		return;
	}
}
E 1
