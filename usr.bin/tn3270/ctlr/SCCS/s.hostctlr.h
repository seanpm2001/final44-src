h52842
s 00002/00002/00194
d D 8.1 93/06/06 16:39:44 bostic 10 9
c 4.4BSD snapshot (revision 8.1); add 1993 to copyright
e
s 00003/00013/00193
d D 4.2 91/04/26 15:40:47 bostic 9 8
c new copyright; att/bsd/shared
e
s 00000/00000/00206
d D 4.1 88/12/04 15:11:58 minshall 8 7
c Release 4.1
e
s 00010/00005/00196
d D 3.3 88/06/29 19:49:49 bostic 7 6
c install approved copyright notice
e
s 00011/00001/00190
d D 3.2 88/03/28 12:34:16 bostic 6 5
c add Berkeley specific header
e
s 00000/00000/00191
d D 3.1 87/08/11 10:18:51 minshall 5 4
c New version numbering.
e
s 00001/00001/00190
d D 1.4 87/07/17 10:03:23 minshall 4 3
c Install sccs headers and copyright notices.
e
s 00028/00003/00163
d D 1.3 87/05/11 11:08:42 minshall 3 2
c 1.  Make ExitString parameters conform.  2.  Support GE order.
e
s 00001/00001/00165
d D 1.2 86/11/25 20:51:39 minshall 2 1
c Fix to correct command hex value.
e
s 00166/00000/00000
d D 1.1 86/11/11 22:54:14 minshall 1 0
c date and time created 86/11/11 22:54:14 by minshall
e
u
U
t
T
I 1
D 9
/*
D 4
 *	@(#)3270.h	3.1  10/29/86
E 4
I 4
D 6
 * %W% (Berkeley) %G%
E 6
I 6
 * Copyright (c) 1988 Regents of the University of California.
E 9
I 9
/*-
D 10
 * Copyright (c) 1988 The Regents of the University of California.
E 9
 * All rights reserved.
E 10
I 10
 * Copyright (c) 1988, 1993
 *	The Regents of the University of California.  All rights reserved.
E 10
 *
D 9
 * Redistribution and use in source and binary forms are permitted
D 7
 * provided that this notice is preserved and that due credit is given
 * to the University of California at Berkeley. The name of the University
 * may not be used to endorse or promote products derived from this
 * software without specific prior written permission. This software
 * is provided ``as is'' without express or implied warranty.
E 7
I 7
 * provided that the above copyright notice and this paragraph are
 * duplicated in all such forms and that any documentation,
 * advertising materials, and other materials related to such
 * distribution and use acknowledge that the software was developed
 * by the University of California, Berkeley.  The name of the
 * University may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
E 9
I 9
 * %sccs.include.redist.c%
E 9
E 7
 *
 *	%W% (Berkeley) %G%
E 6
E 4
 */

#define	INCLUDED_HOST3270

/* define orders given to 3270's */

#define	ORDER_SF	0x1d		/* Start Field */
I 3
#define	ORDER_SFE	0x29		/* Start Field Extended */
E 3
#define	ORDER_SBA	0x11		/* Set Buffer Address (for output) */
I 3
#define	ORDER_SA	0x28		/* Set Attribute */
#define	ORDER_MF	0x2c		/* Modify field */
E 3
#define	ORDER_IC	0x13		/* Insert Cursor (at buffer address) */
#define	ORDER_PT	0x05		/* Program Tab (absurdly complicated) */
#define	ORDER_RA	0x3c		/* Repeat next character to some addr */
D 3
#define	ORDER_SFE	0x29		/* Start Field Extended */
E 3
#define	ORDER_EUA	0x12		/* Null out every unprotected field
					 * to some address.
					 */
D 3
#define	ORDER_MF	0x2c		/* Modify field */
#define	ORDER_SA	0x28		/* Set Attribute */
E 3
I 3
#define	ORDER_GE	0x08		/* Graphics Escape */
E 3
#define	ORDER_YALE	0x2b		/* This is a special YALE order, which
					 * introduces YALE extended orders
					 * (like setting tabs, etc.).
					 */
I 3

/* The following is defined for initialization and error messages. */

struct orders_def {
    int
	code;			/* As in 3270 data stream */
    char
	*short_name,		/* Short name */
	*long_name;		/* Long name */
};

#define	ORDERS_DEF { \
			    ORDER_SF, "SF", "Start Field", \
			    ORDER_SFE, "SFE", "Start Field Extended", \
			    ORDER_SBA, "SBA", "Set Buffer Address", \
			    ORDER_SA, "SA", "Set Attribute", \
			    ORDER_MF, "MF", "Modify Field", \
			    ORDER_IC, "IC", "Insert Cursor", \
			    ORDER_PT, "PT", "Program Tab", \
			    ORDER_RA, "RA", "Repeat to Address", \
			    ORDER_EUA, "EUA", "Erase Unprotected to Address", \
			    ORDER_GE, "GE", "Graphics Escape", \
			    ORDER_YALE, "YALE", "Yale Order" \
			}
E 3


#define	ATTR_RESET		0x00		/* SA only - reset to default */
#	define	ATTR_DEFAULT	0x00		/* reset to default */
						/* Also for 0x41-43 below */
#define	ATTR_FIELD		0xC0		/* Field attributes */
#	define	ATTR_MASK		0xc0	/* control bits */
#	define	ATTR_PROT		0x20	/* protected bit */
#	define	ATTR_NUMERIC		0x10	/* numeric field */
#	define	ATTR_AUTO_SKIP_MASK	0x30	/* mask to check auto skip */
#	define	ATTR_AUTO_SKIP_VALUE	0x30	/* value to have auto skip */
#	define	ATTR_DSPD_MASK		0x0c	/* highlighting, etc. */
#	define	ATTR_DSPD_DNSPD		0x00	/* display, no select */
#	define	ATTR_DSPD_DSPD		0x04	/* display, select */
#	define	ATTR_DSPD_HIGH		0x08	/* highlighted, select */
#	define	ATTR_DSPD_NONDISPLAY	0x0c	/* non-display, no select */
#	define	ATTR_MDT		0x01		/* modified data tag */

#define	ATTR_EXTENDED_HIGHLIGHT	0x41		/* Extended highlighting */
#	define	ATTR_BLINK		0xf1	/* Blinking */
#	define	ATTR_REVERSE_VIDEO	0xf2	/* Reverse video */
#	define	ATTR_UNDERSCORE		0xf3	/* Underline */
#define	ATTR_COLOR		0x42		/* Color */
#	define	ATTR_BLUE		0xf1
#	define	ATTR_RED		0xf2
#	define	ATTR_PINK		0xf3
#	define	ATTR_GREEN		0xf4
#	define	ATTR_TURQUOISE		0xf5
#	define	ATTR_YELLOW		0xf6
#	define	ATTR_WHITE		0xf7	/* for 3279; black for 3287; */
						/* multicolor for triple */
						/* plane symbol */
#define	ATTR_PROGRAMMED_SYMBOLS	0x43		/* Programmed Symbols */
#	define	ATTR_SYMBOL_SET_LOW	0x40	/* Lowest loadable set ID */
#	define	ATTR_SYMBOL_SET_HIGH	0xef	/* Highest loadable set ID */
#	define	ATTR_SYMBOL_SET_APLTEXT	0xf1

/* Non-SNA control unit commands */

#define	CMD_ERASE_ALL_UNPROTECTED	0x0f
#define	CMD_ERASE_WRITE			0x05
#define	CMD_ERASE_WRITE_ALTERNATE	0x0d
#define	CMD_READ_BUFFER			0x02
#define	CMD_READ_MODIFIED		0x06
#define	CMD_WRITE			0x01
#define	CMD_WRITE_STRUCTURED_FIELD	0x11

/* SNA control unit commands */

#define	CMD_SNA_COPY			0xf7
#define	CMD_SNA_ERASE_ALL_UNPROTECTED	0x6f
D 2
#define	CMD_SNA_ERASE_WRITE		0x55
E 2
I 2
#define	CMD_SNA_ERASE_WRITE		0xf5
E 2
#define	CMD_SNA_ERASE_WRITE_ALTERNATE	0x7e
#define	CMD_SNA_READ_BUFFER		0xf2
#define	CMD_SNA_READ_MODIFIED		0xf6
#define	CMD_SNA_READ_MODIFIED_ALL	0x6e
#define	CMD_SNA_WRITE			0xf1
#define	CMD_SNA_WRITE_STRUCTURED_FIELD	0xf3


#define	WCC_RESET	0x40
#define	WCC_ALARM	0x04
#define	WCC_RESTORE	0x02
#define	WCC_RESET_MDT	0x01


/* Special EBCDIC characters unique to a 3270 */

#define	EBCDIC_BLANK	0x40			/* Space */
#define	EBCDIC_CENTSIGN	0x4a			/* Cent sign */
#define	EBCDIC_DUP	0x1c			/* DUP character */
#define	EBCDIC_FM	0x1e			/* Field mark character */
#define	EBCDIC_PERCENT	0x6c			/* Percent sign */
#define	EBCDIC_SLASH	0x61			/* Slash */
#define	EBCDIC_SOH	0x01			/* Start of Heading */
#define	EBCDIC_STX	0x02			/* Start of Text */

/* Structured field types */
#define	SF_3270DS	0x40			/* For write operations */
#define	SF_LPS		0x06			/* Load Programmed Symbols */
#define	SF_SRM		0x09			/* Set Reply Mode */
#define	SF_SWO		0x0b			/* Set Window Origin */
#define	SF_READ_PARTITION	0x01		/* Read Partition (Query) */
#define	SF_ERASE_RESET		0x03		/* Erase (and/or Reset) */
#define	SF_SCS_DATA		0x41		/* SCS Data */
#define	SF_CREATE_PARTITION	0x0c		/* Create a partition */

/* AID characters sent to host.
 *
 * Note that this file (the following entries) are scanned by mkhit.c,
 * and that the format must remain more-or-less consistent
 * (#define\tAID_name\t[\t]*TOKEN)
 */

#define	AID_NONE		0x60		/* No AID (display) */
#define	AID_NONE_PRINTER	0xe8		/* No AID (printer) */

#define	AID_PA1			0x6c
#define	AID_PA2			0x6e
#define	AID_PA3			0x6b
#define	AID_CLEAR		0x6d
#define	AID_TREQ		0xf0
#define	AID_ENTER		0x7d
#define	AID_SELPEN		0x7e	/*
					 * Really, only SELPEN with DESIGNATOR
					 * = space or null
					 */
#define	AID_PF1			0xf1
#define	AID_PF2			0xf2
#define	AID_PF3			0xf3
#define	AID_PF4			0xf4
#define	AID_PF5			0xf5
#define	AID_PF6			0xf6
#define	AID_PF7			0xf7
#define	AID_PF8			0xf8
#define	AID_PF9			0xf9
#define	AID_PF10		0x7a
#define	AID_PF11		0x7b
#define	AID_PF12		0x7c
#define	AID_PF13		0xc1
#define	AID_PF14		0xc2
#define	AID_PF15		0xc3
#define	AID_PF16		0xc4
#define	AID_PF17		0xc5
#define	AID_PF18		0xc6
#define	AID_PF19		0xc7
#define	AID_PF20		0xc8
#define	AID_PF21		0xc9
#define	AID_PF22		0x4a
#define	AID_PF23		0x4b
#define	AID_PF24		0x4c
#define	AID_PF25		0xd1
#define	AID_PF26		0xd2
#define	AID_PF27		0xd3
#define	AID_PF28		0xd4
#define	AID_PF29		0xd5
#define	AID_PF30		0xd6
#define	AID_PF31		0xd7
#define	AID_PF32		0xd8
#define	AID_PF33		0xd9
#define	AID_PF34		0x5a
#define	AID_PF35		0x5b
#define	AID_PF36		0x5c
E 1
