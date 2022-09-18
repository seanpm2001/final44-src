/*-
 * Copyright (c) 1991 The Regents of the University of California.
 * All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Computer Consoles Inc.
 *
 * This module is believed to contain source code proprietary to AT&T.
 * Use and redistribution is subject to the Berkeley Software License
 * Agreement and your Software Agreement with AT&T (Western Electric).
 */

#ifndef lint
static char sccsid[] = "@(#)mclock_.c	5.2 (Berkeley) 4/12/91";
#endif /* not lint */

long int  mclock_()
  {
  int  buf[6];
  times(buf);
  return(buf[0]+buf[2]+buf[3]);
  }
