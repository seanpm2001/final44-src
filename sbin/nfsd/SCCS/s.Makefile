h22311
s 00000/00000/00009
d D 8.1 93/06/05 11:07:44 bostic 6 5
c 4.4BSD snapshot (revision 8.1)
e
s 00001/00001/00008
d D 5.4 93/04/27 18:04:06 bostic 5 3
c daemon(3) now in the C library
e
s 00001/00001/00008
d R 5.4 91/01/15 14:10:26 bostic 4 3
c kernel reorg
e
s 00001/00001/00008
d D 5.3 90/06/29 20:03:11 karels 3 2
c use daemon() call
e
s 00007/00046/00002
d D 5.2 90/05/11 13:00:22 bostic 2 1
c first pass for new make
e
s 00048/00000/00000
d D 5.1 89/07/16 17:36:42 mckusick 1 0
c date and time created 89/07/16 17:36:42 by mckusick
e
u
U
t
T
I 1
D 2
#
# Copyright (c) 1989 Regents of the University of California.
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
E 2
I 2
#	%W% (Berkeley) %G%
E 2

D 2
CFLAGS=	-O -DNFS
LIBC=	/lib/libc.a
SRCS=	nfsd.c
OBJS=
MAN=	nfsd.0
E 2
I 2
PROG=	nfsd
CFLAGS+=-DNFS
MAN8=	nfsd.0
DPADD=	${LIBRPC}
D 3
LDADD=	-lrpc
E 3
I 3
D 5
LDADD=	-lrpc -lutil
E 5
I 5
LDADD=	-lrpc
E 5
E 3
E 2

D 2
all: nfsd

nfsd: ${LIBC}
	${CC} -o $@ ${CFLAGS} $@.c -lrpc

clean:
	rm -f ${OBJS} core nfsd

cleandir: clean
	rm -f ${MAN} tags .depend

depend: ${SRCS}
	mkdep -p ${CFLAGS} ${SRCS}

install: ${MAN}
	install -s -o bin -g bin -m 755 nfsd ${DESTDIR}/sbin
	install -c -o bin -g bin -m 444 ${MAN} ${DESTDIR}/usr/man/cat8

lint: ${SRCS}
	lint ${CFLAGS} ${SRCS}

tags: ${SRCS}
	ctags ${SRCS}
E 2
I 2
.include <bsd.prog.mk>
E 2
E 1
