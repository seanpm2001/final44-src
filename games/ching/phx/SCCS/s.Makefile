h20845
s 00000/00000/00008
d D 8.1 93/05/31 16:46:11 bostic 2 1
c 4.4BSD snapshot (revision 8.1)
e
s 00008/00000/00000
d D 5.1 90/05/11 12:44:59 bostic 1 0
c first pass for new make
e
u
U
t
T
I 1
#	%W% (Berkeley) %G%

PROG=	ching.phx
CFLAGS+=-I${.CURDIR}/..
BINDIR=	/usr/games
NOMAN=	noman

.include <bsd.prog.mk>
E 1
