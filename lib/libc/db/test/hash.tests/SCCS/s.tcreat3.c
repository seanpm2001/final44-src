h23675
s 00005/00005/00074
d D 8.1 93/06/04 15:33:40 bostic 5 4
c 4.4BSD snapshot (revision 8.1); add 1993 to copyright
e
s 00002/00001/00077
d D 5.4 91/09/11 16:03:23 bostic 4 3
c use dbopen, not hash_open
e
s 00005/00031/00073
d D 5.3 91/09/04 12:43:35 bostic 3 2
c clean up SCCS headers, include files
e
s 00029/00003/00075
d D 5.2 91/09/04 12:39:39 bostic 2 1
c Rev #2 (after ANSI/POSIX changes to hash)
e
s 00078/00000/00000
d D 5.1 91/01/31 17:16:24 bostic 1 0
c date and time created 91/01/31 17:16:24 by bostic
e
u
U
t
T
I 1
/*-
D 3
 * Copyright (c) 1990 The Regents of the University of California.
E 3
I 3
D 5
 * Copyright (c) 1991 The Regents of the University of California.
E 3
 * All rights reserved.
E 5
I 5
 * Copyright (c) 1991, 1993
 *	The Regents of the University of California.  All rights reserved.
E 5
 *
 * This code is derived from software contributed to Berkeley by
 * Margo Seltzer.
 *
D 2
 * %sccs.include.redist.c%
E 2
I 2
D 3
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
E 3
I 3
 * %sccs.include.redist.c%
E 3
E 2
 */

#ifndef lint
D 5
char copyright[] =
D 2
"%Z% Copyright (c) 1990 The Regents of the University of California.\n\
E 2
I 2
D 3
"@(#) Copyright (c) 1990 The Regents of the University of California.\n\
E 3
I 3
"%Z% Copyright (c) 1991 The Regents of the University of California.\n\
E 3
E 2
 All rights reserved.\n";
E 5
I 5
static char copyright[] =
"%Z% Copyright (c) 1991, 1993\n\
	The Regents of the University of California.  All rights reserved.\n";
E 5
#endif /* not lint */

#ifndef lint
D 2
static char sccsid[] = "%W% (Berkeley) %G%";
E 2
I 2
D 3
static char sccsid[] = "@(#)tcreat3.c	5.1 (Berkeley) 1/31/91";
E 3
I 3
static char sccsid[] = "%W% (Berkeley) %G%";
E 3
E 2
#endif /* not lint */

#include <sys/types.h>
D 3
#include <stdio.h>
E 3
#include <sys/file.h>
I 3
#include <stdio.h>
E 3
#include <db.h>

#define INITIAL	25000
#define MAXWORDS    25000	       /* # of elements in search table */

char	wp1[8192];
char	wp2[8192];
main(argc, argv)
char **argv;
{
	DBT item, key;
	DB	*dbp;
	HASHINFO ctl;
	FILE *fp;
	int	trash;

	int i = 0;

	argv++;
	ctl.hash = NULL;
	ctl.bsize = atoi(*argv++);
	ctl.ffactor = atoi(*argv++);
	ctl.nelem = atoi(*argv++);
	ctl.lorder = 0;
D 4
	if (!(dbp = hash_open( "hashtest", O_CREAT|O_TRUNC|O_RDWR, 0600, &ctl))){
E 4
I 4
	if (!(dbp = dbopen( "hashtest",
	    O_CREAT|O_TRUNC|O_RDWR, 0600, DB_HASH, &ctl))){
E 4
		/* create table */
		fprintf(stderr, "cannot create: hash table (size %d)\n",
			INITIAL);
		exit(1);
	}

	key.data = wp1;
	item.data = wp2;
	while ( fgets(wp1, 8192, stdin) &&
		fgets(wp2, 8192, stdin) &&
		i++ < MAXWORDS) {
/*
* put info in structure, and structure in the item
*/
		key.size = strlen(wp1);
		item.size = strlen(wp2);

/*
 * enter key/data pair into the table
 */
		if ((dbp->put)(dbp, &key, &item, R_NOOVERWRITE) != NULL) {
			fprintf(stderr, "cannot enter: key %s\n",
				item.data);
			exit(1);
		}			
	}

	(dbp->close)(dbp);
	exit(0);
}
E 1
