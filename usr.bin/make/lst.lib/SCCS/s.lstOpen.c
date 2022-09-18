h08954
s 00000/00000/00055
d D 8.2 95/04/28 17:21:58 christos 5 4
c upgraded to the latest NetBSD version
e
s 00002/00002/00053
d D 8.1 93/06/06 15:11:29 bostic 4 3
c 4.4BSD snapshot (revision 8.1); add 1993 to copyright
e
s 00001/00011/00054
d D 5.3 90/06/01 17:01:05 bostic 3 2
c new copyright notice
e
s 00024/00014/00041
d D 5.2 90/03/11 17:03:22 bostic 2 1
c add Berkeley specific copyright notice
e
s 00055/00000/00000
d D 5.1 90/03/11 16:54:06 bostic 1 0
c release 2.1 of pmake
e
u
U
t
T
I 2
/*
D 4
 * Copyright (c) 1988, 1989, 1990 The Regents of the University of California.
 * All rights reserved.
E 4
I 4
 * Copyright (c) 1988, 1989, 1990, 1993
 *	The Regents of the University of California.  All rights reserved.
E 4
 *
 * This code is derived from software contributed to Berkeley by
 * Adam de Boor.
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
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
E 3
I 3
 * %sccs.include.redist.c%
E 3
 */

#ifndef lint
static char sccsid[] = "%W% (Berkeley) %G%";
#endif /* not lint */

E 2
I 1
/*-
 * LstOpen.c --
 *	Open a list for sequential access. The sequential functions access the
 *	list in a slightly different way. CurPtr points to their idea of the
 *	current node in the list and they access the list based on it.
 *	If the list is circular, Lst_Next and Lst_Prev will go around
 *	the list forever. Lst_IsAtEnd must be used to determine when to stop.
D 2
 *
 * Copyright (c) 1988 by University of California Regents
 *
 * Permission to use, copy, modify, and distribute this
 * software and its documentation for any purpose and without
 * fee is hereby granted, provided that the above copyright
 * notice appears in all copies.  Neither the University of California nor
 * Adam de Boor makes any representations about the suitability of this
 * software for any purpose.  It is provided "as is" without
 * express or implied warranty.
E 2
 */
D 2
#ifndef lint
static char *rcsid =
"$Id: lstOpen.c,v 1.6 88/11/17 20:53:43 adam Exp $ SPRITE (Berkeley)";
#endif lint
E 2

#include	"lstInt.h"

/*-
 *-----------------------------------------------------------------------
 * Lst_Open --
 *	Open a list for sequential access. A list can still be searched,
 *	etc., without confusing these functions.
 *
 * Results:
 *	SUCCESS or FAILURE.
 *
 * Side Effects:
 *	isOpen is set TRUE and curPtr is set to NilListNode so the
 *	other sequential functions no it was just opened and can choose
 *	the first element accessed based on this.
 *
 *-----------------------------------------------------------------------
 */
ReturnStatus
Lst_Open (l)
	register Lst	l;
{
	if (LstValid (l) == FALSE) {
		return (FAILURE);
	}
	((List) l)->isOpen = TRUE;
	((List) l)->atEnd = LstIsEmpty (l) ? Head : Unknown;
	((List) l)->curPtr = NilListNode;

	return (SUCCESS);
}

E 1
