h61787
s 00000/00000/00005
d D 8.1 93/06/06 14:30:07 bostic 3 2
c 4.4BSD snapshot (revision 8.1)
e
s 00002/00045/00003
d D 5.2 90/05/11 15:57:32 bostic 2 1
c first pass for new make
e
s 00048/00000/00000
d D 5.1 89/12/25 12:03:00 bostic 1 0
c date and time created 89/12/25 12:03:00 by bostic
e
u
U
t
T
I 1
D 2
#
# Copyright (c) 1989 The Regents of the University of California.
# All rights reserved.
#
# Redistribution and use in source and binary forms are permitted
# provided that the above copyright notice and this paragraph are
# duplicated in all such forms and that any documentation,
# advertising materials, and other materials related to such
# distribution and use acknowledge that the software was developed
# by the University of California, Berkeley.  The name of the
# University may not be used to endorse or promote products derived
# from this software without specific prior written permission.
# THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
# IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
#
E 2
#	%W% (Berkeley) %G%
D 2
#
E 2

D 2
CFLAGS=	-O
LIBC=	/lib/libc.a
SRCS=	cut.c
OBJS=
MAN=	cut.0
E 2
I 2
PROG=	cut
E 2

D 2
all: cut

cut: ${LIBC}
	${CC} -o $@ ${CFLAGS} $@.c

clean:
	rm -f ${OBJS} core cut

cleandir: clean
	rm -f ${MAN} tags .depend

depend: ${SRCS}
	mkdep -p ${CFLAGS} ${SRCS}

install: ${MAN}
	install -s -o bin -g bin -m 755 cut ${DESTDIR}/usr/bin
	install -c -o bin -g bin -m 444 ${MAN} ${DESTDIR}/usr/man/cat1

lint: ${SRCS}
	lint ${CFLAGS} ${SRCS}

tags: ${SRCS}
	ctags ${SRCS}
E 2
I 2
.include <bsd.prog.mk>
E 2
E 1
