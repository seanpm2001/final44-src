h04444
s 00004/00046/00002
d D 5.3 90/05/31 16:52:31 bostic 3 2
c pmake
e
s 00001/00001/00047
d D 5.2 89/09/18 13:07:38 bostic 2 1
c replace with des, move to /usr/old
e
s 00048/00000/00000
d D 5.1 88/10/22 16:02:03 bostic 1 0
c date and time created 88/10/22 16:02:03 by bostic
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
SRCS=	crypt.c
OBJS=
MAN=	crypt.0
E 3
I 3
PROG=	crypt
MAN1=	crypt.0
E 3

D 3
all: crypt

crypt: ${LIBC}
	${CC} -o $@ ${CFLAGS} $@.c

clean:
	rm -f ${OBJS} core crypt

cleandir: clean
	rm -f ${MAN} tags .depend

depend: ${SRCS}
	mkdep -p ${CFLAGS} ${SRCS}

install: ${MAN}
D 2
	install -s -o bin -g bin -m 755 crypt ${DESTDIR}/usr/bin
E 2
I 2
	install -s -o bin -g bin -m 755 crypt ${DESTDIR}/usr/old
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
