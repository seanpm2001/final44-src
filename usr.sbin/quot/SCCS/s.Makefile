h28465
s 00000/00000/00006
d D 8.1 93/06/06 14:04:57 bostic 7 6
c 4.4BSD snapshot (revision 8.1)
e
s 00000/00002/00006
d D 5.5 93/04/27 18:18:24 bostic 6 4
c pwcache(3) now in the C library
e
s 00001/00000/00008
d R 5.5 91/01/14 17:54:14 bostic 5 4
c kernel reorg
e
s 00002/00000/00006
d D 5.4 90/06/23 14:31:06 bostic 4 3
c needs libutil now
e
s 00004/00046/00002
d D 5.3 90/05/11 16:09:47 bostic 3 2
c first pass for new make
e
s 00001/00001/00047
d D 5.2 89/05/15 08:23:48 bostic 2 1
c file reorg, pathnames.h
e
s 00048/00000/00000
d D 5.1 88/10/19 12:06:42 bostic 1 0
c date and time created 88/10/19 12:06:42 by bostic
e
u
U
t
T
I 1
D 3
#
# Copyright (c) 1988 Regents of the University of California.
# All rights reserved.
#
# Redistribution and use in source and binary forms are permitted
# provided that the above copyright notice and this paragraph are
# duplicated in all such forms and that any documentation, advertising
# materials, and other materials related to such redistribution and
# use acknowledge that the software was developed by the University
# of California, Berkeley.  The name of the University may not be
# used to endorse or promote products derived from this software
# without specific prior written permission.  THIS SOFTWARE IS PROVIDED
# ``AS IS'' AND WITHOUT ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
# WITHOUT LIMITATION, THE IMPLIED WARRANTIES OF MERCHANTIBILITY AND
# FITNESS FOR A PARTICULAR PURPOSE.
#
# %W% (Berkeley) %G%
#
E 3
I 3
#	%W% (Berkeley) %G%
E 3

D 3
CFLAGS=	-O
LIBC=	/lib/libc.a
SRCS=	quot.c
OBJS=
MAN=	quot.0
E 3
I 3
PROG=	quot
I 4
D 6
DPADD=	${LIBUTIL}
LDADD=	-lutil
E 6
E 4
MAN8=	quot.0
E 3

D 3
all: quot

quot: ${LIBC}
	${CC} -o $@ ${CFLAGS} $@.c

clean:
	rm -f ${OBJS} core quot

cleandir: clean
	rm -f ${MAN} tags .depend

depend: ${SRCS}
	mkdep -p ${CFLAGS} ${SRCS}

install: ${MAN}
D 2
	install -s -o bin -g bin -m 755 quot ${DESTDIR}/etc
E 2
I 2
	install -s -o bin -g bin -m 755 quot ${DESTDIR}/usr/sbin
E 2
	install -c -o bin -g bin -m 444 ${MAN} ${DESTDIR}/usr/man/cat8

lint: ${SRCS}
	lint ${CFLAGS} ${SRCS}

tags: ${SRCS}
	ctags ${SRCS}
E 3
I 3
.include <bsd.prog.mk>
E 3
E 1