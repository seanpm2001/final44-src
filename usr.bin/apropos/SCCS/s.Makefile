h26223
s 00000/00000/00007
d D 8.1 93/06/06 15:22:08 bostic 6 5
c 4.4BSD snapshot (revision 8.1)
e
s 00000/00001/00007
d D 5.4 93/06/06 15:22:05 bostic 5 3
c move man/{apropos,whatis,man} up a level
e
s 00000/00000/00008
d R 8.1 93/06/06 15:21:33 bostic 4 3
c 4.4BSD snapshot (revision 8.1)
e
s 00001/00000/00007
d D 5.3 90/05/27 15:19:21 bostic 3 2
c move to subdirectory of usr.bin/man
e
s 00004/00045/00003
d D 5.2 90/05/11 15:13:47 bostic 2 1
c first pass for new make
e
s 00048/00000/00000
d D 5.1 90/03/11 15:08:02 bostic 1 0
c date and time created 90/03/11 15:08:02 by bostic
e
u
U
t
T
I 1
D 2
#
# Copyright (c) 1987 Regents of the University of California.
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
# WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
#
E 2
#	%W% (Berkeley) %G%
D 2
#
CFLAGS=	-O
LIBC=	/lib/libc.a
VPATH=	../man
E 2
I 2

PROG=	apropos
E 2
SRCS=	apropos.c config.c
D 2
OBJS=	apropos.o config.o
MAN=	apropos.0
E 2
I 2
.PATH:	${.CURDIR}/../man
E 2

I 3
D 5
.include "../../Makefile.inc"
E 5
E 3
D 2
all: apropos

apropos: ${LIBC} ${OBJS}
	${CC} -o $@ ${CFLAGS} ${OBJS}

clean:
	rm -f core apropos ${OBJS}

cleandir: clean
	rm -f ${MAN} tags .depend

depend:
	mkdep ${CFLAGS} ${SRCS}

install: ${MAN}
	install -s -o bin -g bin -m 755 apropos ${DESTDIR}/usr/bin
	install -c -o bin -g bin -m 444 ${MAN} ${DESTDIR}/usr/man/cat1

lint:
	lint ${CFLAGS} ${SRCS}

tags:
	ctags ${SRCS}
E 2
I 2
.include <bsd.prog.mk>
E 2
E 1
