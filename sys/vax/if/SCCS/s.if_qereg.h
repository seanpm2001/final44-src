h48507
s 00001/00011/00144
d D 7.3 90/06/28 22:53:29 bostic 5 4
c new copyright notice
e
s 00021/00001/00134
d D 7.2 88/08/04 15:30:10 bostic 4 3
c add Berkeley specific copyright
e
s 00000/00000/00135
d D 7.1 86/06/05 01:38:18 mckusick 3 2
c 4.3BSD release version
e
s 00002/00003/00133
d D 6.1 86/05/09 16:23:00 karels 2 1
c rework for 4.3: rm decnet, binary stuff, add ns, use new if_uba
e
s 00136/00000/00000
d D 1.1 86/04/28 13:44:39 kjd 1 0
c date and time created 86/04/28 13:44:39 by kjd
e
u
U
t
T
I 1
D 2

E 2
D 4
/*	%W% (Berkeley) %G% */
E 4
I 4
/*
 * Copyright (c) 1988 Regents of the University of California.
 * All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Digital Equipment Corp.
 *
D 5
 * Redistribution and use in source and binary forms are permitted
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
E 5
I 5
 * %sccs.include.redist.c%
E 5
 *
 *	%W% (Berkeley) %G%
 */
E 4

/* @(#)if_qereg.h	1.2 (ULTRIX) 1/3/85 */
 
/****************************************************************
 *								*
 *        Licensed from Digital Equipment Corporation 		*
 *                       Copyright (c) 				*
 *               Digital Equipment Corporation			*
 *                   Maynard, Massachusetts 			*
 *                         1985, 1986 				*
 *                    All rights reserved. 			*
 *								*
 *        The Information in this software is subject to change *
 *   without notice and should not be construed as a commitment *
 *   by  Digital  Equipment  Corporation.   Digital   makes  no *
 *   representations about the suitability of this software for *
 *   any purpose.  It is supplied "As Is" without expressed  or *
 *   implied  warranty. 					*
 *								*
 *        If the Regents of the University of California or its *
 *   licensees modify the software in a manner creating  	*
 *   diriviative copyright rights, appropriate copyright  	*
 *   legends may be placed on  the drivative work in addition   *
 *   to that set forth above. 					*
 *								*
 ****************************************************************/
/* ---------------------------------------------------------------------
 * Modification History 
 *
 *  13 Feb. 84 -- rjl
 *
 *	Initial version of driver. derived from IL driver.
 * 
 * ---------------------------------------------------------------------
 */
 
/*
 * Digital Q-BUS to NI Adapter 
 */
struct qedevice {
	u_short	qe_sta_addr[2]; 	/* Station address (actually 6 	*/
D 2
	u_short	qe_rcvlist_lo; 		/* Recieve list lo address 	*/
	u_short	qe_rcvlist_hi; 		/* Recieve list hi address 	*/
E 2
I 2
	u_short	qe_rcvlist_lo; 		/* Receive list lo address 	*/
	u_short	qe_rcvlist_hi; 		/* Receive list hi address 	*/
E 2
	u_short	qe_xmtlist_lo;		/* Transmit list lo address 	*/
	u_short	qe_xmtlist_hi;		/* Transmit list hi address 	*/
	u_short	qe_vector;		/* Interrupt vector 		*/
	u_short	qe_csr;			/* Command and Status Register 	*/
};
 
/*
 * Command and status bits (csr)
 */
#define QE_RCV_ENABLE	0x0001		/* Receiver enable		*/
#define QE_RESET	0x0002		/* Software reset		*/
#define QE_NEX_MEM_INT	0x0004		/* Non existant mem interrupt	*/
#define QE_LOAD_ROM	0x0008		/* Load boot/diag from rom	*/
#define QE_XL_INVALID	0x0010		/* Transmit list invalid	*/
#define QE_RL_INVALID	0x0020		/* Receive list invalid		*/
#define QE_INT_ENABLE	0x0040		/* Interrupt enable		*/
#define QE_XMIT_INT	0x0080		/* Transmit interrupt		*/
#define QE_ILOOP 	0x0100		/* Internal loopback		*/
#define QE_ELOOP	0x0200		/* External loopback		*/
#define QE_STIM_ENABLE	0x0400		/* Sanity timer enable		*/
#define QE_POWERUP	0x1000		/* Tranceiver power on		*/
#define QE_CARRIER	0x2000		/* Carrier detect		*/
#define QE_RCV_INT	0x8000		/* Receiver interrupt		*/
 
/*
 * Transmit and receive ring discriptor ---------------------------
 *
 * The QNA uses the flag, status1 and the valid bit as a handshake/semiphore
 * mechinism. 
 * 
 * The flag word is written on ( bits 15,15 set to 1 ) when it reads the
 * descriptor. If the valid bit is set it considers the address to be valid.
 * When it uses the buffer pointed to by the valid address it sets status word
 * one.
 */
struct qe_ring	{
	u_short qe_flag;		/* Buffer utilization flags	*/
	u_short qe_addr_hi:6,		/* Hi order bits of buffer addr	*/
	      qe_odd_begin:1,		/* Odd byte begin and end (xmit)*/
	      qe_odd_end:1,
	      qe_fill1:4,
	      qe_setup:1,		/* Setup packet			*/
	      qe_eomsg:1,		/* End of message flag		*/
	      qe_chain:1,		/* Chain address instead of buf */
	      qe_valid:1;		/* Address field is valid	*/
	u_short qe_addr_lo;		/* Low order bits of address	*/
	short qe_buf_len;		/* Negative buffer length	*/
	u_short qe_status1;		/* Status word one		*/
	u_short qe_status2;		/* Status word two		*/
};
 
/*
 * Status word definations (receive)
 *	word1
 */
#define QE_OVF			0x0001	/* Receiver overflow		*/
#define QE_CRCERR		0x0002	/* CRC error			*/
#define QE_FRAME		0x0004	/* Framing alignment error	*/
#define QE_SHORT		0x0008	/* Packet size < 10 bytes	*/
#define QE_RBL_HI		0x0700	/* Hi bits of receive len	*/
#define QE_RUNT			0x0800	/* Runt packet			*/
#define QE_DISCARD		0x1000	/* Discard the packet		*/
#define QE_ESETUP		0x2000	/* Looped back setup or eloop	*/
#define QE_ERROR		0x4000	/* Receiver error		*/
#define QE_LASTNOT		0x8000	/* Not the last in the packet	*/
/*	word2								*/
#define QE_RBL_LO		0x00ff	/* Low bits of receive len	*/
 
/*
 * Status word definations (transmit)
 *	word1
 */
#define QE_CCNT			0x00f0	/* Collision count this packet	*/
#define QE_FAIL			0x0100	/* Heart beat check failure	*/
#define QE_ABORT		0x0200	/* Transmission abort		*/
#define QE_STE16		0x0400	/* Sanity timer default on	*/
#define QE_NOCAR		0x0800	/* No carrier			*/
#define QE_LOSS			0x1000	/* Loss of carrier while xmit	*/
/*	word2								*/
#define QE_TDR			0x3fff	/* Time domain reflectometry	*/
 
/*
 * General constant definations
 */
#define QEALLOC 		0	/* Allocate an mbuf		*/
#define QENOALLOC		1	/* No mbuf allocation		*/
#define QEDEALLOC		2	/* Release an mbuf chain	*/
 
#define QE_NOTYET		0x8000	/* Descriptor not in use yet	*/
#define QE_INUSE		0x4000	/* Descriptor being used by QNA	*/
#define QE_MASK			0xc000	/* Lastnot/error/used mask	*/
E 1