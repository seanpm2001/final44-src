/*
 *  Author: cck    Date: 87/08/28 11:14:05  
 *  Header: aufsicon.c,v 1.7 87/08/28 11:14:05 cck Rel  
 *  Revision: 1.7  
*/

/*
 * aufsicon.c - aufs icon.
 *
 *
 * AppleTalk package for UNIX (4.2 BSD).
 *
 * Copyright (c) 1986, 1987 by The Trustees of Columbia University in the
 * City of New York.
 *
 * Edit History:
 *
 *  March 1987 	Schilit		Created.
 *  April 1987  CCKim           Clean up, put rep. of icon in
 *
 */
#include <netat/appletalk.h>

/* This is ICN# = 128 from aufsicon.rc */
export byte aufsicon[] = {
  /* icon */
  0x00,0x0c,0x01,0x80,		/*             XX         XX        */
  0x00,0x18,0xf8,0xc0,		/*            XX   XXXXX   XX       */
  0x00,0x15,0x05,0x40,		/*            X X X     X X X       */
  0x00,0x12,0x72,0x40,		/*            X  X  XXX  X  X       */
  0x00,0x0d,0xc9,0x40,		/*             XX XXX  X  X X       */
  0x00,0x13,0x24,0xc0,		/*            X  XX  X  X  XX       */
  0x00,0x22,0x14,0x40,		/*           X   X    X X   X       */
  0x00,0x3b,0x94,0x40,		/*           XXX XXX  X X   X       */
  0x00,0x3b,0x94,0x40,		/*           XXX XXX  X X   X       */
  0x00,0x3b,0x94,0x40,		/*           XXX XXX  X X   X       */
  0x00,0x22,0x14,0x40,		/*           X   X    X X   X       */
  0x00,0x22,0x24,0x44,		/*           X   X   X  X   X   X   */
  0x00,0x1f,0xd8,0x49,		/*            XXXXXXX XX    X  X  X */
  0x78,0x08,0x80,0x4a,		/*  XXXX       X   X        X  X X  */
  0x84,0x08,0x9c,0x8c,		/* X    X      X   X  XXX  X   XX   */
  0xff,0xe7,0x08,0x99,		/* XXXXXXXXXXX  XXX    X   X  XX  X */
  0x80,0x12,0x11,0xe6,		/* X          X  X    X   XXXX  XX  */
  0x81,0x11,0xfe,0x80,		/* X      X   X   XXXXXXXX X        */
  0x82,0x12,0x08,0x86,		/* X     X    X  X     X   X    XX  */
  0x84,0x13,0x90,0x89,		/* X    X     X  XXX  X    X   X  X */
  0x88,0x14,0x71,0x89,		/* X   X      X X   XXX   XX   X  X */
  0x92,0x94,0x4e,0x92,		/* X  X  X X  X X   X  XXX X  X  X  */
  0x80,0x16,0x40,0xa4,		/* X          X XX  X      X X  X   */
  0xff,0xf3,0x90,0xc4,		/* XXXXXXXXXXXX  XXX  X    XX   X   */
  0x05,0x06,0x01,0x88,		/*      X X     XX        XX   X    */
  0x0f,0x89,0x01,0x2a,		/*     XXXXX   X  X       X  X X X  */
  0x08,0x80,0x91,0x1c,		/*     X   X       X  X   X   XXX   */
  0x0f,0x80,0x91,0x08,		/*     XXXXX       X  X   X    X    */
  0x0a,0x80,0x91,0x00,		/*     X X X       X  X   X         */
  0xb2,0x7f,0xff,0xfd,		/* X XX  X  XXXXXXXXXXXXXXXXXXXXX X */
  0x05,0x00,0x00,0x00,		/*      X X                         */
  0xb8,0xff,0xff,0xfd,		/* X XXX   XXXXXXXXXXXXXXXXXXXXXX X */
  /* Mask */
  0x00,0x0c,0x01,0x80,		/*             XX         XX        */
  0x00,0x18,0xf8,0xc0,		/*            XX   XXXXX   XX       */
  0x00,0x1d,0xfd,0xc0,		/*            XXX XXXXXXX XXX       */
  0x00,0x1f,0xff,0xc0,		/*            XXXXXXXXXXXXXXX       */
  0x00,0x0f,0xff,0xc0,		/*             XXXXXXXXXXXXXX       */
  0x00,0x1f,0xff,0xc0,		/*            XXXXXXXXXXXXXXX       */
  0x00,0x3f,0xff,0xc0,		/*           XXXXXXXXXXXXXXXX       */
  0x00,0x3f,0xff,0xc0,		/*           XXXXXXXXXXXXXXXX       */
  0x00,0x3f,0xff,0xc0,		/*           XXXXXXXXXXXXXXXX       */
  0x00,0x3f,0xff,0xc0,		/*           XXXXXXXXXXXXXXXX       */
  0x00,0x3f,0xff,0xc0,		/*           XXXXXXXXXXXXXXXX       */
  0x00,0x3f,0xff,0xc4,		/*           XXXXXXXXXXXXXXXX   X   */
  0x00,0x1f,0xff,0xc9,		/*            XXXXXXXXXXXXXXX  X  X */
  0x78,0x0f,0xff,0xca,		/*  XXXX       XXXXXXXXXXXXXX  X X  */
  0xfc,0x0f,0xff,0x8c,		/* XXXXXX      XXXXXXXXXXXXX   XX   */
  0xff,0xe7,0xff,0x99,		/* XXXXXXXXXXX  XXXXXXXXXXXX  XX  X */
  0xff,0xf3,0xff,0xe6,		/* XXXXXXXXXXXX  XXXXXXXXXXXXX  XX  */
  0xff,0xf1,0xff,0x80,		/* XXXXXXXXXXXX   XXXXXXXXXX        */
  0xff,0xf3,0xff,0x86,		/* XXXXXXXXXXXX  XXXXXXXXXXX    XX  */
  0xff,0xf3,0xff,0x89,		/* XXXXXXXXXXXX  XXXXXXXXXXX   X  X */
  0xff,0xf7,0xff,0x89,		/* XXXXXXXXXXXX XXXXXXXXXXXX   X  X */
  0xff,0xf7,0xff,0x92,		/* XXXXXXXXXXXX XXXXXXXXXXXX  X  X  */
  0xff,0xf7,0xff,0xa4,		/* XXXXXXXXXXXX XXXXXXXXXXXX X  X   */
  0xff,0xf3,0xff,0xc4,		/* XXXXXXXXXXXX  XXXXXXXXXXXX   X   */
  0x07,0x07,0xff,0x88,		/*      XXX     XXXXXXXXXXXX   X    */
  0x0f,0x89,0xff,0x2a,		/*     XXXXX   X  XXXXXXXXX  X X X  */
  0x0f,0x80,0xff,0x1c,		/*     XXXXX       XXXXXXXX   XXX   */
  0x0f,0x80,0xff,0x08,		/*     XXXXX       XXXXXXXX    X    */
  0x0a,0x80,0xff,0x00,		/*     X X X       XXXXXXXX         */
  0xb2,0x7f,0xff,0xfd,		/* X XX  X  XXXXXXXXXXXXXXXXXXXXX X */
  0x05,0x00,0x00,0x00,		/*      X X                         */
  0xb8,0xff,0xff,0xfd		/* X XXX   XXXXXXXXXXXXXXXXXXXXXX X */
};

export int aufsiconsize = sizeof(aufsicon);

