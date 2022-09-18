h25298
s 00000/00003/00007
d D 8.2 94/05/23 14:34:55 mckusick 5 4
c syntax errors
e
s 00000/00000/00010
d D 8.1 93/06/08 18:03:43 bostic 4 3
c 4.4BSD snapshot (revision 8.1)
e
s 00002/00002/00008
d D 5.3 93/06/05 15:46:34 bostic 3 2
c new doc.mk; document numbering changes for 4.4BSD
e
s 00004/00019/00006
d D 5.2 90/06/30 15:04:04 bostic 2 1
c new doc makefiles
e
s 00025/00000/00000
d D 5.1 90/06/07 18:15:09 bostic 1 0
c date and time created 90/06/07 18:15:09 by bostic
e
u
U
t
T
I 1
D 2
#
# Copyright (c) 1986 Regents of the University of California.
# All rights reserved.  The Berkeley software License Agreement
# specifies the terms and conditions for redistribution.
#
E 2
#	%W% (Berkeley) %G%
D 2
#
E 2
I 2

D 3
DIR=	ps2/05.iosys
E 3
I 3
DIR=	psd/03.iosys
E 3
E 2
SRCS=	iosys
MACROS=	-ms
D 2
PRINTER=Pdp
TROFF=	ditroff -${PRINTER}
E 2

D 2
paper:	paper.${PRINTER}
	lpr -${PRINTER} -n paper.${PRINTER}

E 2
D 3
paper.${PRINTER}: ${SRCS}
E 3
I 3
D 5
paper.ps: ${SRCS}
E 3
D 2
	${TROFF} ${MACROS} -t ${SRCS} > paper.${PRINTER}
E 2
I 2
	${ROFF} ${SRCS} > ${.TARGET}
E 2

E 5
D 2
clean:
	rm -f paper.[PT]* *.spell errs Errs make.out

spell:	${SRCS}
	@for i in ${SRCS}; do \
		echo $$i; spell $$i | sort | comm -23 - spell.ok > $$i.spell; \
	done
E 2
I 2
.include <bsd.doc.mk>
E 2
E 1
