h18882
s 00000/00000/00007
d D 8.1 93/06/06 16:13:10 bostic 5 4
c 4.4BSD snapshot (revision 8.1)
e
s 00002/00000/00005
d D 5.4 91/07/03 16:33:00 marc 4 3
c use pty klibrary
e
s 00003/00046/00002
d D 5.3 90/05/11 15:13:42 bostic 3 2
c first pass for new make
e
s 00001/00001/00047
d D 5.2 89/05/11 09:47:43 bostic 2 1
c file reorg, pathnames.h, paths.h
e
s 00048/00000/00000
d D 5.1 88/10/21 13:52:59 bostic 1 0
c date and time created 88/10/21 13:52:59 by bostic
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
SRCS=	script.c
OBJS=
MAN=	script.0
E 3
I 3
PROG=	script
I 4
LDADD=	-lutil
DPADD=	${LIBUTIL}
E 4
E 3

D 3
all: script

script: ${LIBC}
	${CC} -o $@ ${CFLAGS} $@.c

clean:
	rm -f ${OBJS} core script

cleandir: clean
	rm -f ${MAN} tags .depend

depend: ${SRCS}
	mkdep -p ${CFLAGS} ${SRCS}

install: ${MAN}
D 2
	install -s -o bin -g bin -m 755 script ${DESTDIR}/usr/ucb
E 2
I 2
	install -s -o bin -g bin -m 755 script ${DESTDIR}/usr/bin
E 2
	install -c -o bin -g bin -m 444 ${MAN} ${DESTDIR}/usr/man/cat1

lint: ${SRCS}
	lint ${CFLAGS} ${SRCS}

tags: ${SRCS}
	ctags ${SRCS}
E 3
I 3
.include <bsd.prog.mk>
E 3
E 1