h07213
s 00002/00002/00047
d D 8.1 93/06/04 12:54:59 bostic 8 7
c 4.4BSD snapshot (revision 8.1); add 1993 to copyright
e
s 00004/00002/00045
d D 5.7 92/06/25 09:34:15 bostic 7 6
c KNF, integrate with standard include files (quad -> quad_t etc.)
e
s 00001/00001/00046
d D 5.6 92/06/21 10:13:40 bostic 6 5
c GCC2 changed the type of the shift count; make this work since we
c still use both compilers.  This should be ripped out as soon as possible.
c From Chris Torek
e
s 00001/00001/00046
d D 5.5 92/06/02 20:41:58 mckusick 5 4
c shifts parameters are quads too
e
s 00031/00052/00016
d D 5.4 92/06/02 13:35:02 bostic 4 3
c new version from Chris Torek
e
s 00024/00000/00044
d D 5.3 92/05/12 18:21:58 bostic 3 2
c Prepend the gcc-2.1 release copyright notice to all source code
e
s 00001/00001/00043
d D 5.2 92/05/12 16:44:12 bostic 2 1
c BITS_PER_UNIT -> NBBY
e
s 00044/00000/00000
d D 5.1 92/05/12 15:19:57 bostic 1 0
c put on a Berkeley copyright notice -- original didn't have a GNU
c copyright, I'll add one as soon as I find out which one to add
c date and time created 92/05/12 15:19:57 by bostic
e
u
U
t
T
I 1
/*-
D 8
 * Copyright (c) 1992 The Regents of the University of California.
 * All rights reserved.
E 8
I 8
 * Copyright (c) 1992, 1993
 *	The Regents of the University of California.  All rights reserved.
E 8
 *
I 4
 * This software was developed by the Computer Systems Engineering group
 * at Lawrence Berkeley Laboratory under DARPA contract BG 91-66 and
 * contributed to Berkeley.
 *
E 4
 * %sccs.include.redist.c%
 */

#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "%W% (Berkeley) %G%";
#endif /* LIBC_SCCS and not lint */

I 3
D 4
/* Copyright (C) 1989, 1992 Free Software Foundation, Inc.
E 4
I 4
#include "quad.h"
E 4

D 4
This file is part of GNU CC.

GNU CC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GNU CC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU CC; see the file COPYING.  If not, write to
the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.  */

/* As a special exception, if you link this library with files
   compiled with GCC to produce an executable, this does not cause
   the resulting executable to be covered by the GNU General Public License.
   This exception does not however invalidate any other reasons why
   the executable file might be covered by the GNU General Public License.  */

E 3
#include "longlong.h"

long long
__ashrdi3 (u, b1)
     long long u;
     long long b1;
E 4
I 4
/*
 * Shift a (signed) quad value right (arithmetic shift right).
 */
D 7
quad
D 5
__ashrdi3(quad a, register unsigned int shift)
E 5
I 5
D 6
__ashrdi3(quad a, u_quad shift)
E 6
I 6
__ashrdi3(quad a, qshift_t shift)
E 7
I 7
quad_t
__ashrdi3(a, shift)
	quad_t a;
	qshift_t shift;
E 7
E 6
E 5
E 4
{
D 4
  long_long w;
  unsigned long carries;
  int bm;
  long_long uu;
  int b = b1;
E 4
I 4
	union uu aa;
E 4

D 4
  if (b == 0)
    return u;
E 4
I 4
	aa.q = a;
	if (shift >= LONG_BITS) {
		long s;
E 4

D 4
  uu.ll = u;

D 2
  bm = (sizeof (int) * BITS_PER_UNIT) - b;
E 2
I 2
  bm = (sizeof (int) * NBBY) - b;
E 2
  if (bm <= 0)
    {
      w.s.high = uu.s.high >> 31; /* just to make w.s.high 1..1 or 0..0 */
      w.s.low = uu.s.high >> -bm;
    }
  else
    {
      carries = (unsigned long)uu.s.high << bm;
      w.s.high = uu.s.high >> b;
      w.s.low = ((unsigned long)uu.s.low >> b) | carries;
    }

  return w.ll;
E 4
I 4
		/*
		 * Smear bits rightward using the machine's right-shift
		 * method, whether that is sign extension or zero fill,
		 * to get the `sign word' s.  Note that shifting by
		 * LONG_BITS is undefined, so we shift (LONG_BITS-1),
		 * then 1 more, to get our answer.
		 */
		s = (aa.sl[H] >> (LONG_BITS - 1)) >> 1;
		aa.ul[L] = shift >= QUAD_BITS ? s :
		    aa.sl[H] >> (shift - LONG_BITS);
		aa.ul[H] = s;
	} else if (shift > 0) {
		aa.ul[L] = (aa.ul[L] >> shift) |
		    (aa.ul[H] << (LONG_BITS - shift));
		aa.sl[H] >>= shift;
	}
	return (aa.q);
E 4
}
E 1
