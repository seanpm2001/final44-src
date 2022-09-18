h46556
s 00002/00002/00268
d D 8.1 93/06/10 23:18:09 bostic 6 5
c 4.4BSD snapshot (revision 8.1); add 1993 to copyright
e
s 00012/00012/00258
d D 7.5 93/05/25 11:51:23 bostic 5 4
c trailing comment after #else or #endif
e
s 00010/00001/00260
d D 7.4 91/05/06 18:58:43 bostic 4 3
c new copyright; att/bsd/shared
e
s 00008/00001/00253
d D 7.3 90/06/22 15:39:18 sklower 3 2
c merge in UW isis changes; my merge is untested, but SCCS id's
c must be assigned before I leave today :-(
e
s 00021/00013/00233
d D 7.2 89/04/22 11:53:07 sklower 2 1
c Checkpoint as of first successful tp connection, before posix, &
c before mckusick starts incorporating gnodes
e
s 00246/00000/00000
d D 7.1 88/12/14 15:29:18 sklower 1 0
c date and time created 88/12/14 15:29:18 by sklower
e
u
U
t
T
I 4
/*-
D 6
 * Copyright (c) 1991 The Regents of the University of California.
 * All rights reserved.
E 6
I 6
 * Copyright (c) 1991, 1993
 *	The Regents of the University of California.  All rights reserved.
E 6
 *
 * %sccs.include.redist.c%
 *
 *	%W% (Berkeley) %G%
 */

E 4
I 1
D 2
/***********************************************************
		Copyright IBM Corporation 1987
E 2
I 2
D 3
/*********************************************************** Copyright IBM Corporation 1987
E 3
I 3
/*****************************************************************
				Copyright IBM Corporation 1987
E 3
E 2

                      All Rights Reserved

Permission to use, copy, modify, and distribute this software and its 
documentation for any purpose and without fee is hereby granted, 
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in 
supporting documentation, and that the name of IBM not be
used in advertising or publicity pertaining to distribution of the
software without specific, written prior permission.  

IBM DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
IBM BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.

******************************************************************/

/*
 * ARGO Project, Computer Sciences Dept., University of Wisconsin - Madison
 */
/* 
 * $Header: argo_debug.h,v 4.6 88/07/19 15:53:40 hagens Exp $
 * $Source: /usr/argo/sys/netiso/RCS/argo_debug.h,v $
I 2
D 4
 *	%W% (Berkeley) %G%
E 4
E 2
 */
I 4

E 4
#ifndef __ARGO_DEBUG__
#define __ARGO_DEBUG__

I 2
#define dump_buf(a, b) Dump_buf((caddr_t)(a), (int)(b))
E 2

/***********************************************
 * Lint stuff
 **********************************************/
#if	defined(lint)
/* 
 * lint can't handle the flaky vacuous definitions 
 * of IFDEBUG, ENDDEBUG, etc.
 */
D 2
#ifndef TPPT
#define TPPT
#endif TPPT

#ifndef ARGO_DEBUG
#define ARGO_DEBUG
#endif ARGO_DEBUG

E 2
D 5
#endif	defined(lint)
E 5
I 5
#endif	/* defined(lint) */
E 5

/***********************************************
 * DEBUG ON:
 **********************************************/
I 2
#ifndef ARGO_DEBUG
#define ARGO_DEBUG
D 5
#endif ARGO_DEBUG
E 5
I 5
#endif /* ARGO_DEBUG */
E 5
E 2

I 2

E 2
#ifdef ARGO_DEBUG
I 2
/*
    #ifndef TPPT
    #define TPPT
    #endif TPPT

    #ifndef TP_PERF_MEAS
    #define TP_PERF_MEAS
    #endif TP_PERF_MEAS
*/

E 2
unsigned char	argo_debug[128];

#define IFDEBUG(ascii) \
	if(argo_debug[ascii]) { 
#define ENDDEBUG  ; }

D 5
#else  ARGO_DEBUG
E 5
I 5
#else  /* ARGO_DEBUG */
E 5

/***********************************************
 * DEBUG OFF:
 **********************************************/

#ifndef STAR
#define STAR *
D 5
#endif	STAR
E 5
I 5
#endif	/* STAR */
E 5
#define IFDEBUG(ascii)	 //*beginning of comment*/STAR
#define ENDDEBUG	 STAR/*end of comment*//

D 5
#endif ARGO_DEBUG
E 5
I 5
#endif /* ARGO_DEBUG */
E 5

/***********************************************
 * ASSERT 
 **********************************************/
#ifdef ARGO_DEBUG

#ifndef lint
#define ASSERT(phrase) \
if( !(phrase) ) printf("ASSERTION NOT VALID at line %d file %s\n",__LINE__,__FILE__)
D 5
#else lint
E 5
I 5
#else /* lint */
E 5
#define ASSERT(phrase) /* phrase */
D 5
#endif lint
E 5
I 5
#endif /* lint */
E 5

D 5
#else ARGO_DEBUG
E 5
I 5
#else /* ARGO_DEBUG */
E 5

#define ASSERT(phrase) /* phrase */

D 5
#endif ARGO_DEBUG
E 5
I 5
#endif /* ARGO_DEBUG */
E 5


/***********************************************
 * CLNP DEBUG OPTIONS
 **********************************************/
#define	D_INPUT			'\1'
/* clnp input */
#define	D_OUTPUT		'\2'
/* clnp output */
#define	D_ROUTE			'\3'
/* clnp routing */
#define	D_CTLINPUT		'\4'
/* clnp control input */
#define	D_CTLOUTPUT		'\5'
/* clnp control output */
#define D_OPTIONS		'\6'
/* clnp options */
#define	D_IOCTL			'\7'
/* iso ioctls */
#define D_ETHER			'\10'
/* clnp over ethernet */
#define D_TOKEN			'\11'
/* clnp over token ring */
#define D_ADCOM			'\12'
/* clnp over the adcom */
#define D_ISO			'\13'	
/* iso address family */
#define	D_FORWARD		'\14'
/* clnp forwarding */
#define	D_DUMPOUT		'\15'
/* dump clnp outgoing packets */
#define	D_DUMPIN		'\16'	
/* dump clnp input packets */
#define D_DISCARD		'\17'	
/* debug clnp packet discard/er function */
#define D_FRAG			'\20'	
/* clnp fragmentation */
#define	D_REASS			'\21'	
/* clnp reassembly */

char *clnp_iso_addrp();

/***********************************************
 * ESIS DEBUG OPTIONS
 **********************************************/
#define	D_ESISOUTPUT	'\30'
#define	D_ESISINPUT		'\31'
#define D_SNPA			'\32'
I 3

/***********************************************
 * ISIS DEBUG OPTIONS
 **********************************************/
#define D_ISISOUTPUT	'\40'
#define D_ISISINPUT		'\41'
E 3

/***********************************************
 * EON DEBUG OPTION
 **********************************************/
#define	D_EON			'\57'

/***********************************************
 * CONS DEBUG OPTIONS
 **********************************************/

#define D_ECNWORK		'\60'
#define D_ECNOUT		'\61'
#define D_ECNFIN		'\62'
#define D_ECNDWN		'\63'
#define D_ECNUTIL		'\64'

#define D_INCOMING		'\70'
#define D_CDATA			'\71'
#define D_CFIND			'\72'
#define D_CDUMP_REQ		'\73'
#define D_CADDR			'\74'
#define D_CCONS			'\75'
#define D_CCONN			'\76'


/***********************************************
 * TP DEBUG OPTIONS
 **********************************************/

#define D_SETPARAMS		'\137'
#define D_RTT 			'\140'

#define D_ACKRECV 		'\141'
#define D_ACKSEND 		'\142'
#define D_CONN 			'\143'
#define D_CREDIT 		'\144'
#define D_DATA 			'\145'
#define D_DRIVER 		'\146'

#define D_EMIT 			'\147'
#define D_ERROR_EMIT 	'\150'
#define D_TPINPUT 		'\151'
#define D_INDICATION 	'\152'
#define D_CHKSUM 		'\153'

#define D_RENEG 		'\154'
#define D_PERF_MEAS 	'\155'
#define D_MBUF_MEAS 	'\156'
#define D_RTC 			'\157'
#define D_SB 			'\160'

#define D_DISASTER_CHECK '\161'
#define D_REQUEST 		'\162'
#define D_STASH 		'\163'
#define D_NEWSOCK 		'\164'
#define D_TIMER 		'\165'

#define D_TPIOCTL 		'\166'
#define D_SIZE_CHECK 	'\167'
#define D_2ER 			'\170'
#define D_DISASTER_CHECK_W '\171'

#define D_XPD 			'\172'
#define D_SYSCALL 		'\173'
#define D_DROP 			'\174'
#define D_ZDREF 		'\175'
#define D_TPISO			'\176'
#define D_QUENCH		'\177'

void dump_mbuf();

/***********************************************
 * New mbuf types for debugging w/ netstat -m
I 2
 * This messes up 4.4 malloc for now. need bigger
 * mbtypes array for now.
E 2
 **********************************************/
D 2
#ifdef ARGO_DEBUG
E 2
I 2
#ifdef notdef
E 2

#define 	TPMT_DATA	0x21
#define 	TPMT_RCVRTC	0x42
#define 	TPMT_SNDRTC	0x41
#define 	TPMT_TPHDR	0x22
#define 	TPMT_IPHDR	0x32
#define 	TPMT_SONAME	0x28
#define 	TPMT_EOT	0x40
#define 	TPMT_XPD	0x44
#define 	TPMT_PCB	0x23
#define 	TPMT_PERF	0x45

D 5
#else ARGO_DEBUG
E 5
I 5
#else /* ARGO_DEBUG */
E 5

#define 	TPMT_DATA	MT_DATA
#define 	TPMT_RCVRTC	MT_DATA
#define 	TPMT_SNDRTC	MT_DATA
#define 	TPMT_IPHDR	MT_HEADER
#define 	TPMT_TPHDR	MT_HEADER
#define 	TPMT_SONAME	MT_SONAME
/* MT_EOT and MT_XPD are defined in tp_param.h */
D 2
#define 	TPMT_EOT	MT_EOT
#define 	TPMT_XPD	MT_XPD
E 2
I 2
#define 	TPMT_XPD	MT_OOBDATA
E 2
#define 	TPMT_PCB	MT_PCB
#define 	TPMT_PERF	MT_PCB

D 5
#endif ARGO_DEBUG
E 5
I 5
#endif /* ARGO_DEBUG */
E 5

D 5
#endif __ARGO_DEBUG__
E 5
I 5
#endif /* __ARGO_DEBUG__ */
E 5
E 1
