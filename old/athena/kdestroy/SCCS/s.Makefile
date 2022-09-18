h65130
s 00001/00001/00048
d D 1.2 89/02/05 16:03:07 kfall 2 1
c install depends on kdestroy
e
s 00049/00000/00000
d D 1.1 89/02/04 14:16:37 kfall 1 0
c date and time created 89/02/04 14:16:37 by kfall
e
u
U
t
T
I 1
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
# @(#)Makefile	5.1 (Berkeley) 10/18/88
#

CFLAGS=	-O -DKERBEROS
LIBC=	/lib/libc.a
LIBS=	/usr/lib/libkrb.a /usr/lib/libdes.a
SRCS=	kdestroy.c
OBJS=
MAN=	kdestroy.0

all: kdestroy

kdestroy: ${LIBS} ${LIBC}
	${CC} -o $@ ${CFLAGS} $@.c -ldes -lkrb

clean:
	rm -f ${OBJS} core kdestroy

cleandir: clean
	rm -f ${MAN} tags .depend

depend: ${SRCS}
	mkdep -p ${CFLAGS} ${SRCS}

D 2
install: ${MAN}
E 2
I 2
install: ${MAN} kdestroy
E 2
	install -s -o bin -g bin -m 755 kdestroy ${DESTDIR}/usr/athena
	install -c -o bin -g bin -m 444 ${MAN} ${DESTDIR}/usr/man/cat1

lint: ${SRCS}
	lint ${CFLAGS} ${SRCS}

tags: ${SRCS}
	ctags ${SRCS}
E 1
