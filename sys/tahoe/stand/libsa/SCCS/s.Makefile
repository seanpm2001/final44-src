h23888
s 00010/00000/00000
d D 5.1 91/05/08 22:01:22 bostic 1 0
c date and time created 91/05/08 22:01:22 by bostic
e
u
U
t
T
I 1
#	%W% (Berkeley) %G%

LIB=	sa
SRCS=	bmap.c close.c dev.c getfile.c gets.c ioctl.c lseek.c open.c \
	printf.c read.c stat.c sys.c write.c
.PATH:	../../../stand
NOPROFILE=noprofile

.include <bsd.lib.mk>

E 1
