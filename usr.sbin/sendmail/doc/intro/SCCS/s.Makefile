h03038
s 00001/00000/00012
d D 8.2 94/02/28 10:18:06 eric 9 8
c remove target before building in case it isn't owned by us
e
s 00001/00001/00011
d D 8.1 93/06/08 17:24:59 bostic 8 6
c document numbering changes for 4.4BSD
e
s 00000/00000/00012
d R 8.1 93/06/08 14:12:35 bostic 7 6
c 4.4BSD snapshot (revision 8.1)
e
s 00001/00001/00011
d D 5.6 93/06/05 17:06:29 eric 6 5
c make intro.ps, not intro.${PRINTER}
e
s 00001/00001/00011
d D 5.5 93/06/05 16:45:39 bostic 5 4
c new doc.mk; document numbering changes for 4.4BSD
e
s 00003/00001/00009
d D 5.4 93/06/01 21:04:34 eric 4 3
c new doc scheme
e
s 00004/00031/00006
d D 5.3 90/06/30 15:06:03 bostic 3 2
c new doc makefiles
e
s 00014/00003/00023
d D 5.2 89/01/23 09:41:55 bostic 2 1
c add Berkeley specific copyright
e
s 00026/00000/00000
d D 5.1 89/01/23 09:41:32 bostic 1 0
c date and time created 89/01/23 09:41:32 by bostic
e
u
U
t
T
I 1
D 3
#
D 2
# Copyright (c) 1986 Regents of the University of California.
# All rights reserved.  The Berkeley software License Agreement
# specifies the terms and conditions for redistribution.
E 2
I 2
# Copyright (c) 1986 The Regents of the University of California.
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
E 2
#
E 3
#	%W% (Berkeley) %G%
D 3
#
E 3
I 3

D 8
DIR=	smm/16.sendmail
E 8
I 8
DIR=	smm/09.sendmail
E 8
E 3
SRCS=	intro.me
MACROS=	-me
D 3
PRINTER=Pdp
TROFF=	ditroff -${PRINTER}
PIC=	pic -${PRINTER}
E 3

D 3
paper:	paper.${PRINTER}
	lpr -${PRINTER} -n paper.${PRINTER}

E 3
D 4
paper.${PRINTER}: ${SRCS}
E 4
I 4
D 6
all: intro.${PRINTER}
E 6
I 6
all: intro.ps
E 6

D 5
intro.${PRINTER}: ${SRCS}
E 5
I 5
intro.ps: ${SRCS}
I 9
	rm -f ${.TARGET}
E 9
E 5
E 4
D 3
	${PIC} ${SRCS} | ${TROFF} ${MACROS} -t > paper.${PRINTER}
E 3
I 3
	${PIC} ${SRCS} | ${ROFF} > ${.TARGET}
E 3

D 3
clean:
	rm -f paper.[PT]* *.spell errs Errs make.out

spell:	${SRCS}
	@for i in ${SRCS}; do \
		echo $$i; spell $$i | sort | comm -23 - spell.ok > $$i.spell; \
	done
E 3
I 3
.include <bsd.doc.mk>
E 3
E 1
