/* 
 * Mach Operating System
 * Copyright (c) 1991 Carnegie-Mellon University
 * Copyright (c) 1990 Carnegie-Mellon University
 * Copyright (c) 1989 Carnegie-Mellon University
 * Copyright (c) 1988 Carnegie-Mellon University
 * Copyright (c) 1987 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	if_de.c,v $
 * Revision 2.5  93/05/15  18:59:03  mrt
 * 	machparam.h -> machspl.h
 * 
 * Revision 2.4  91/05/13  06:07:55  af
 * 	Rid of the "CMU" conditional once and forall.
 * 	[91/05/12  16:37:25  af]
 * 
 * Revision 2.3  91/05/08  13:29:02  dbg
 * 	Added volatile declarations.  Moved br and cvec to memory.
 * 	[91/03/26            dbg]
 * 
 * 	Include vax/machparam.h for SPL definitions.
 * 	[91/03/20            dbg]
 * 
 * Revision 2.2  90/06/02  15:08:58  rpd
 * 	Converted to new IPC.
 * 	[90/03/26  23:06:10  rpd]
 * 
 * Revision 2.1  89/08/03  16:39:14  rwd
 * Created.
 * 
 * 14-Nov-88  David Golub (dbg) at Carnegie-Mellon University
 *	Mangled for MACH_KERNEL.
 *
 * 18-May-88  Paul Parker (parker) at Carnegie-Mellon University
 *	Upgraded to UCB version 7.2.
 *	[ V5.1(XF24) ]
 *
 * 06-May-87  Mike Accetta (mja) at Carnegie-Mellon University
 *	DLI:  Add support for new direct data-link interface protocol.
 *	CS_GENERIC:  Add support for IFF_ALLMULTI and IFF_PROMISC
 *	interface status flags; expand error printout in dewait() to
 *	try and identify why re-initialization fails from time to time.
 *	[ V5.1(F10) ]
 *
 */ 
 
#include <dli.h>

/*
 * Copyright (c) 1982, 1986 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)if_de.c	7.2 (Berkeley) 7/18/86
 */
#include <de.h>
#if NDE > 0

/*
 * DEC DEUNA interface
 *
 *	Lou Salkind
 *	New York University
 *
 * TODO:
 *	timeout routine (get statistics)
 */
#ifdef	MACH_KERNEL
#include <kern/time_out.h>
#include <sys/syslog.h>

#include <device/device_types.h>
#include <device/errno.h>
#include <device/io_req.h>
#include <device/if_hdr.h>
#include <device/if_ether.h>
#include <device/net_status.h>

#include <vax/vm_defs.h>
#include <vax/machspl.h>
#else	MACH_KERNEL
#include "../machine/pte.h"

#include "param.h"
#include "systm.h"
#include "mbuf.h"
#include "buf.h"
#include "protosw.h"
#include "socket.h"
#include "vmmac.h"
#include "ioctl.h"
#include "errno.h"
#include "syslog.h"

#include "../net/if.h"
#include "../net/netisr.h"
#include "../net/route.h"

#ifdef INET
#include "../netinet/in.h"
#include "../netinet/in_systm.h"
#include "../netinet/in_var.h"
#include "../netinet/ip.h"
#include "../netinet/if_ether.h"
#endif

#ifdef NS
#include "../netns/ns.h"
#include "../netns/ns_if.h"
#endif

#if	DLI
#include "net/dli_var.h"
struct dli_var de_dlv[NDE];
#endif	DLI

#include "../vax/cpu.h"
#endif	MACH_KERNEL
#include <vax/mtpr.h>

#include <vaxif/if_dereg.h>
#include <vaxif/if_uba.h>

#include <vaxuba/ubareg.h>
#include <vaxuba/ubavar.h>

#define	NXMT	3	/* number of transmit buffers */
#define	NRCV	7	/* number of receive buffers (must be > 1) */

int	dedebug = 0;

int	deprobe(), deattach(), deintr();
struct	uba_device *deinfo[NDE];
u_short destd[] = { 0 };
struct	uba_driver dedriver =
	{ deprobe, 0, deattach, 0, destd, "de", deinfo };
int	deinit(),deoutput(),deioctl(),dereset();


/*
 * Ethernet software status per interface.
 *
 * Each interface is referenced by a network interface structure,
 * ds_if, which the routing code uses to locate the interface.
 * This structure contains the output queue for the interface, its address, ...
 * We also have, for each interface, a UBA interface structure, which
 * contains information about the UNIBUS resources held by the interface:
 * map registers, buffered data paths, etc.  Information is cached in this
 * structure for use by the if_uba.c routines in running the interface
 * efficiently.
 */
struct	de_softc {
#ifdef	MACH_KERNEL
	struct	ifnet	ds_if;		/* generic interface header	*/
	u_char	ds_addr[6];		/* ethernet hardware address	*/
#else	MACH_KERNEL
	struct	arpcom ds_ac;		/* Ethernet common part */
#define	ds_if	ds_ac.ac_if		/* network-visible interface */
#define	ds_addr	ds_ac.ac_enaddr		/* hardware Ethernet address */
#endif	MACH_KERNEL
	int	ds_flags;
#define	DSF_LOCK	1		/* lock out destart */
#define	DSF_RUNNING	2		/* board is enabled */
#define	DSF_SETADDR	4		/* physical address is changed */
	int	ds_ubaddr;		/* map info for incore structs */
	struct	ifubinfo ds_deuba;	/* unibus resource structure */
	struct	ifrw ds_ifr[NRCV];	/* unibus receive maps */
	struct	ifxmt ds_ifw[NXMT];	/* unibus xmt maps */
	/* the following structures are always mapped in */
	struct	de_pcbb ds_pcbb;	/* port control block */
	struct	de_ring ds_xrent[NXMT];	/* transmit ring entrys */
	struct	de_ring ds_rrent[NRCV];	/* receive ring entrys */
	struct	de_udbbuf ds_udbbuf;	/* UNIBUS data buffer */
	/* end mapped area */
#define	INCORE_BASE(p)	((char *)&(p)->ds_pcbb)
#define	RVAL_OFF(n)	((char *)&de_softc[0].n - INCORE_BASE(&de_softc[0]))
#define	LVAL_OFF(n)	((char *)de_softc[0].n - INCORE_BASE(&de_softc[0]))
#define	PCBB_OFFSET	RVAL_OFF(ds_pcbb)
#define	XRENT_OFFSET	LVAL_OFF(ds_xrent)
#define	RRENT_OFFSET	LVAL_OFF(ds_rrent)
#define	UDBBUF_OFFSET	RVAL_OFF(ds_udbbuf)
#define	INCORE_SIZE	RVAL_OFF(ds_xindex)
	int	ds_xindex;		/* UNA index into transmit chain */
	int	ds_rindex;		/* UNA index into receive chain */
	int	ds_xfree;		/* index for next transmit buffer */
	int	ds_nxmit;		/* # of transmits in progress */
#if	MACH
	short	ds_mode;		/* current receive mode */
#endif	MACH
} de_softc[NDE];

extern volatile int	br, cvec;

deprobe(reg)
	caddr_t reg;
{
	register struct dedevice *addr = (struct dedevice *)reg;
	register i;

#ifdef lint
	i = 0; derint(i); deintr(i);
#endif

	/*
	 * Make sure self-test is finished before we screw with the board.
	 * Self-test on a DELUA can take 15 seconds (argh).
	 */
	for (i = 0;
	     i < 160 &&
	     (addr->pcsr0 & PCSR0_FATI) == 0 &&
	     (addr->pcsr1 & PCSR1_STMASK) == STAT_RESET;
	     ++i)
		DELAY(100000);
	if ((addr->pcsr0 & PCSR0_FATI) != 0 ||
	    (addr->pcsr1 & PCSR1_STMASK) != STAT_READY)
		return(0);

	addr->pcsr0 = 0;
	DELAY(100);
	addr->pcsr0 = PCSR0_RSET;
	while ((addr->pcsr0 & PCSR0_INTR) == 0)
		;
	/* make board interrupt by executing a GETPCBB command */
	addr->pcsr0 = PCSR0_INTE;
	addr->pcsr2 = 0;
	addr->pcsr3 = 0;
	addr->pcsr0 = PCSR0_INTE|CMD_GETPCBB;
	DELAY(100000);
	return(1);
}

/*
 * Interface exists: make available by filling in network interface
 * record.  System will initialize the interface when it is ready
 * to accept packets.  We get the ethernet address here.
 */
deattach(ui)
	struct uba_device *ui;
{
	register struct de_softc *ds = &de_softc[ui->ui_unit];
	register struct ifnet *ifp = &ds->ds_if;
	register struct dedevice *addr = (struct dedevice *)ui->ui_addr;
	int csr1;

	ifp->if_unit = ui->ui_unit;
#ifdef	MACH_KERNEL
	ifp->if_header_size = sizeof(struct ether_header);
	ifp->if_header_format = HDR_ETHERNET;
	ifp->if_address_size = 6;
#else	MACH_KERNEL
	ifp->if_name = "de";
#endif	MACH_KERNEL
	ifp->if_mtu = ETHERMTU;
	ifp->if_flags = IFF_BROADCAST;

	/*
	 * What kind of a board is this?
	 * The error bits 4-6 in pcsr1 are a device id as long as
	 * the high byte is zero.
	 */
	csr1 = addr->pcsr1;
	if (csr1 & 0xff60)
		printf("de%d: broken\n", ui->ui_unit);
	else if (csr1 & 0x10)
		printf("de%d: delua\n", ui->ui_unit);
	else
		printf("de%d: deuna\n", ui->ui_unit);

	/*
	 * Reset the board and temporarily map
	 * the pcbb buffer onto the Unibus.
	 */
	addr->pcsr0 = 0;		/* reset INTE */
	DELAY(100);
	addr->pcsr0 = PCSR0_RSET;
	(void)dewait(ui, "reset");

	ds->ds_ubaddr = uballoc(ui->ui_ubanum, (char *)&ds->ds_pcbb,
		sizeof (struct de_pcbb), 0);
	addr->pcsr2 = ds->ds_ubaddr & 0xffff;
	addr->pcsr3 = (ds->ds_ubaddr >> 16) & 0x3;
	addr->pclow = CMD_GETPCBB;
	(void)dewait(ui, "pcbb");

	ds->ds_pcbb.pcbb0 = FC_RDPHYAD;
	addr->pclow = CMD_GETCMD;
	(void)dewait(ui, "read addr ");

	ubarelse(ui->ui_ubanum, &ds->ds_ubaddr);
 	bcopy((caddr_t)&ds->ds_pcbb.pcbb2, (caddr_t)ds->ds_addr,
	    sizeof (ds->ds_addr));
	printf("de%d: hardware address %s\n", ui->ui_unit,
		ether_sprintf(ds->ds_addr));
#ifdef	MACH_KERNEL
	ifp->if_address = (char *)&ds->ds_addr[0];
#else	MACH_KERNEL
	ifp->if_init = deinit;
	ifp->if_output = deoutput;
	ifp->if_ioctl = deioctl;
	ifp->if_reset = dereset;
#endif	MACH_KERNEL
	ds->ds_deuba.iff_flags = UBA_CANTWAIT;
#ifdef notdef
	/* CAN WE USE BDP's ??? */
	ds->ds_deuba.iff_flags |= UBA_NEEDBDP;
#endif
#ifdef	MACH_KERNEL
	if_init_queues(ifp);
#else	MACH_KERNEL
	if_attach(ifp);
#endif	MACH_KERNEL
}

/*
 * Reset of interface after UNIBUS reset.
 * If interface is on specified uba, reset its state.
 */
#ifdef	MACH_KERNEL
deallreset(uban)
	int	uban;
{
	register int	unit;
	for (unit = 0; unit < NDE; unit++)
	    dereset(unit, uban);
}
#endif	MACH_KERNEL

dereset(unit, uban)
	int unit, uban;
{
	register struct uba_device *ui;

	if (unit >= NDE || (ui = deinfo[unit]) == 0 || ui->ui_alive == 0 ||
	    ui->ui_ubanum != uban)
		return;
	printf(" de%d", unit);
	de_softc[unit].ds_if.if_flags &= ~IFF_RUNNING;
	de_softc[unit].ds_flags &= ~(DSF_LOCK | DSF_RUNNING);
	deinit(unit);
}

/*
 * Initialization of interface; clear recorded pending
 * operations, and reinitialize UNIBUS usage.
 */
deinit(unit)
	int unit;
{
	register struct de_softc *ds = &de_softc[unit];
	register struct uba_device *ui = deinfo[unit];
	register struct dedevice *addr;
	register struct ifrw *ifrw;
	register struct ifxmt *ifxp;
	struct ifnet *ifp = &ds->ds_if;
	int s;
	struct de_ring *rp;
	int incaddr;

#ifdef	MACH_KERNEL
#else	MACH_KERNEL
	/* not yet, if address still unknown */
	if (ifp->if_addrlist == (struct ifaddr *)0)
		return;
#endif	MACH_KERNEL

	if (ds->ds_flags & DSF_RUNNING)
		return;
	if ((ifp->if_flags & IFF_RUNNING) == 0) {
		if (if_ubaminit(&ds->ds_deuba, ui->ui_ubanum,
		    sizeof (struct ether_header), (int)btoc(ETHERMTU),
#ifdef	MACH_KERNEL
		    ds->ds_ifr, NRCV, ds->ds_ifw, NXMT, 0
#else	MACH_KERNEL
		    ds->ds_ifr, NRCV, ds->ds_ifw, NXMT
#endif	MACH_KERNEL
				) == 0) { 
			printf("de%d: can't initialize\n", unit);
			ds->ds_if.if_flags &= ~IFF_UP;
			return;
		}
#ifdef	MACH_KERNEL
		ifp->if_alloc_size = ds->ds_deuba.iff_size;
#endif	MACH_KERNEL
		ds->ds_ubaddr = uballoc(ui->ui_ubanum, INCORE_BASE(ds),
			INCORE_SIZE, 0);
	}
	addr = (struct dedevice *)ui->ui_addr;

	/* set the pcbb block address */
	incaddr = ds->ds_ubaddr + PCBB_OFFSET;
	addr->pcsr2 = incaddr & 0xffff;
	addr->pcsr3 = (incaddr >> 16) & 0x3;
	addr->pclow = 0;	/* reset INTE */
	DELAY(100);
	addr->pclow = CMD_GETPCBB;
	(void)dewait(ui, "pcbb");

	/* set the transmit and receive ring header addresses */
	incaddr = ds->ds_ubaddr + UDBBUF_OFFSET;
	ds->ds_pcbb.pcbb0 = FC_WTRING;
	ds->ds_pcbb.pcbb2 = incaddr & 0xffff;
	ds->ds_pcbb.pcbb4 = (incaddr >> 16) & 0x3;

	incaddr = ds->ds_ubaddr + XRENT_OFFSET;
	ds->ds_udbbuf.b_tdrbl = incaddr & 0xffff;
	ds->ds_udbbuf.b_tdrbh = (incaddr >> 16) & 0x3;
	ds->ds_udbbuf.b_telen = sizeof (struct de_ring) / sizeof (short);
	ds->ds_udbbuf.b_trlen = NXMT;
	incaddr = ds->ds_ubaddr + RRENT_OFFSET;
	ds->ds_udbbuf.b_rdrbl = incaddr & 0xffff;
	ds->ds_udbbuf.b_rdrbh = (incaddr >> 16) & 0x3;
	ds->ds_udbbuf.b_relen = sizeof (struct de_ring) / sizeof (short);
	ds->ds_udbbuf.b_rrlen = NRCV;

	addr->pclow = CMD_GETCMD;
	(void)dewait(ui, "wtring");

	/* initialize the mode - enable hardware padding */
	ds->ds_pcbb.pcbb0 = FC_WTMODE;
	/* let hardware do padding - set MTCH bit on broadcast */
#if	MACH
	ds->ds_pcbb.pcbb2 = MOD_TPAD|MOD_HDX|ds->ds_mode;
#else	MACH
	ds->ds_pcbb.pcbb2 = MOD_TPAD|MOD_HDX;
#endif	MACH
	addr->pclow = CMD_GETCMD;
	(void)dewait(ui, "wtmode");

	/* set up the receive and transmit ring entries */
	ifxp = &ds->ds_ifw[0];
	for (rp = &ds->ds_xrent[0]; rp < &ds->ds_xrent[NXMT]; rp++) {
		rp->r_segbl = ifxp->ifw_info & 0xffff;
		rp->r_segbh = (ifxp->ifw_info >> 16) & 0x3;
		rp->r_flags = 0;
		ifxp++;
	}
	ifrw = &ds->ds_ifr[0];
	for (rp = &ds->ds_rrent[0]; rp < &ds->ds_rrent[NRCV]; rp++) {
		rp->r_slen = sizeof (struct de_buf);
		rp->r_segbl = ifrw->ifrw_info & 0xffff;
		rp->r_segbh = (ifrw->ifrw_info >> 16) & 0x3;
		rp->r_flags = RFLG_OWN;		/* hang receive */
		ifrw++;
	}

	/* start up the board (rah rah) */
	s = splimp();
	ds->ds_rindex = ds->ds_xindex = ds->ds_xfree = ds->ds_nxmit = 0;
	ds->ds_if.if_flags |= IFF_RUNNING;
	addr->pclow = PCSR0_INTE;		/* avoid interlock */
	destart(unit);				/* queue output packets */
	ds->ds_flags |= DSF_RUNNING;		/* need before de_setaddr */
	if (ds->ds_flags & DSF_SETADDR)
		de_setaddr(ds->ds_addr, unit);
	addr->pclow = CMD_START | PCSR0_INTE;
	splx(s);
#if	DLI
	dli_init(&de_dlv[ui->ui_unit], ifp, ds->ds_addr, etherbroadcastaddr,
		 sizeof(etherbroadcastaddr), sizeof(struct ether_header));
#endif	DLI
}

#ifdef	MACH_KERNEL
/*ARGSUSED*/
deopen(dev, flag)
	dev_t	dev;
	int	flag;
{
	register int unit = minor(dev);
	register struct uba_device *ui;
	register struct de_softc *ds;

	if (unit >= NDE || (ui = deinfo[unit]) == 0 || ui->ui_alive == 0)
		return (ENXIO);

	ds = &de_softc[unit];
	ds->ds_if.if_flags |= IFF_UP;
	deinit(unit);
	return (0);
}
#endif	MACH_KERNEL

/*
 * Setup output on interface.
 * Get another datagram to send off of the interface queue,
 * and map it to the interface before starting the output.
 */
destart(unit)
	int unit;
{
        int len;
	struct uba_device *ui = deinfo[unit];
	struct dedevice *addr = (struct dedevice *)ui->ui_addr;
	register struct de_softc *ds = &de_softc[unit];
	register struct de_ring *rp;
#ifdef	MACH_KERNEL
	io_req_t	m;
	u_long		buf_addr;
#else	MACH_KERNEL
	struct mbuf *m;
#endif	MACH_KERNEL
	register int nxmit;

	/*
	 * the following test is necessary, since
	 * the code is not reentrant and we have
	 * multiple transmission buffers.
	 */
	if (ds->ds_flags & DSF_LOCK)
		return;
	for (nxmit = ds->ds_nxmit; nxmit < NXMT; nxmit++) {
		IF_DEQUEUE(&ds->ds_if.if_snd, m);
		if (m == 0)
			break;
		rp = &ds->ds_xrent[ds->ds_xfree];
		if (rp->r_flags & XFLG_OWN)
			panic("deuna xmit in progress");
#ifdef	MACH_KERNEL
		if_ubaput(&ds->ds_deuba, &ds->ds_ifw[ds->ds_xfree], m,
			  &buf_addr, &len);
		rp->r_segbl = buf_addr & 0xffff;
		rp->r_segbh = (buf_addr >> 16) & 0x3;
#else	MACH_KERNEL
		len = if_ubaput(&ds->ds_deuba, &ds->ds_ifw[ds->ds_xfree], m);
#endif	MACH_KERNEL
		if (ds->ds_deuba.iff_flags & UBA_NEEDBDP)
			UBAPURGE(ds->ds_deuba.iff_uba,
			ds->ds_ifw[ds->ds_xfree].ifw_bdp);
		rp->r_slen = len;
		rp->r_tdrerr = 0;
		rp->r_flags = XFLG_STP|XFLG_ENP|XFLG_OWN;

		ds->ds_xfree++;
		if (ds->ds_xfree == NXMT)
			ds->ds_xfree = 0;
	}
	if (ds->ds_nxmit != nxmit) {
		ds->ds_nxmit = nxmit;
		if (ds->ds_flags & DSF_RUNNING)
			addr->pclow = PCSR0_INTE|CMD_PDMD;
	}
}

/*
 * Command done interrupt.
 */
deintr(unit)
	int unit;
{
	struct uba_device *ui = deinfo[unit];
	register struct dedevice *addr = (struct dedevice *)ui->ui_addr;
	register struct de_softc *ds = &de_softc[unit];
	register struct de_ring *rp;
	register struct ifxmt *ifxp;
	short csr0;

	/* save flags right away - clear out interrupt bits */
	csr0 = addr->pcsr0;
	addr->pchigh = csr0 >> 8;


	ds->ds_flags |= DSF_LOCK;	/* prevent entering destart */
	/*
	 * if receive, put receive buffer on mbuf
	 * and hang the request again
	 */
	derecv(unit);

	/*
	 * Poll transmit ring and check status.
	 * Be careful about loopback requests.
	 * Then free buffer space and check for
	 * more transmit requests.
	 */
	for ( ; ds->ds_nxmit > 0; ds->ds_nxmit--) {
		rp = &ds->ds_xrent[ds->ds_xindex];
		if (rp->r_flags & XFLG_OWN)
			break;
		ds->ds_if.if_opackets++;
		ifxp = &ds->ds_ifw[ds->ds_xindex];
		/* check for unusual conditions */
		if (rp->r_flags & (XFLG_ERRS|XFLG_MTCH|XFLG_ONE|XFLG_MORE)) {
			if (rp->r_flags & XFLG_ERRS) {
				/* output error */
				ds->ds_if.if_oerrors++;
				if (dedebug)
			printf("de%d: oerror, flags=%b tdrerr=%b (len=%d)\n",
				    unit, rp->r_flags, XFLG_BITS,
				    rp->r_tdrerr, XERR_BITS, rp->r_slen);
			} else if (rp->r_flags & XFLG_ONE) {
				/* one collision */
				ds->ds_if.if_collisions++;
			} else if (rp->r_flags & XFLG_MORE) {
				/* more than one collision */
				ds->ds_if.if_collisions += 2;	/* guess */
			} else if (rp->r_flags & XFLG_MTCH) {
				/* received our own packet */
				ds->ds_if.if_ipackets++;
				deread(ds, &ifxp->ifrw,
				    rp->r_slen - sizeof (struct ether_header));
			}
		}
		if (ifxp->ifw_xtofree) {
#ifdef	MACH_KERNEL
			iodone(ifxp->ifw_xtofree);
#else	MACH_KERNEL
			m_freem(ifxp->ifw_xtofree);
#endif	MACH_KERNEL
			ifxp->ifw_xtofree = 0;
		}
		/* check if next transmit buffer also finished */
		ds->ds_xindex++;
		if (ds->ds_xindex == NXMT)
			ds->ds_xindex = 0;
	}
	ds->ds_flags &= ~DSF_LOCK;
	destart(unit);

	if (csr0 & PCSR0_RCBI) {
		if (dedebug)
			log(LOG_WARNING, "de%d: buffer unavailable\n", unit);
		addr->pclow = PCSR0_INTE|CMD_PDMD;
	}
}

/*
 * Ethernet interface receiver interface.
 * If input error just drop packet.
 * Otherwise purge input buffered data path and examine 
 * packet to determine type.  If can't determine length
 * from type, then have to drop packet.  Othewise decapsulate
 * packet based on type and pass to type specific higher-level
 * input routine.
 */
derecv(unit)
	int unit;
{
	register struct de_softc *ds = &de_softc[unit];
	register struct de_ring *rp;
	int len;

	rp = &ds->ds_rrent[ds->ds_rindex];
	while ((rp->r_flags & RFLG_OWN) == 0) {
		ds->ds_if.if_ipackets++;
		if (ds->ds_deuba.iff_flags & UBA_NEEDBDP)
			UBAPURGE(ds->ds_deuba.iff_uba,
			ds->ds_ifr[ds->ds_rindex].ifrw_bdp);
		len = (rp->r_lenerr&RERR_MLEN) - sizeof (struct ether_header)
			- 4;	/* don't forget checksum! */
		/* check for errors */
		if ((rp->r_flags & (RFLG_ERRS|RFLG_FRAM|RFLG_OFLO|RFLG_CRC)) ||
		    (rp->r_flags&(RFLG_STP|RFLG_ENP)) != (RFLG_STP|RFLG_ENP) ||
		    (rp->r_lenerr & (RERR_BUFL|RERR_UBTO|RERR_NCHN)) ||
		    len < ETHERMIN || len > ETHERMTU) {
			ds->ds_if.if_ierrors++;
			if (dedebug)
			printf("de%d: ierror, flags=%b lenerr=%b (len=%d)\n",
				unit, rp->r_flags, RFLG_BITS, rp->r_lenerr,
				RERR_BITS, len);
		} else
			deread(ds, &ds->ds_ifr[ds->ds_rindex], len);

		/* hang the receive buffer again */
		rp->r_lenerr = 0;
		rp->r_flags = RFLG_OWN;

		/* check next receive buffer */
		ds->ds_rindex++;
		if (ds->ds_rindex == NRCV)
			ds->ds_rindex = 0;
		rp = &ds->ds_rrent[ds->ds_rindex];
	}
}

/*
 * Pass a packet to the higher levels.
 * We deal with the trailer protocol here.
 */
deread(ds, ifrw, len)
	register struct de_softc *ds;
	struct ifrw *ifrw;
	int len;
{
#ifdef	MACH_KERNEL
	if_ubaget(&ds->ds_deuba, ifrw, len + sizeof(struct ether_header),
		  &ds->ds_if);
#else	MACH_KERNEL
	struct ether_header *eh;
    	struct mbuf *m;
	int off, resid;
	int s;
	register struct ifqueue *inq;

	/*
	 * Deal with trailer protocol: if type is trailer type
	 * get true type from first 16-bit word past data.
	 * Remember that type was trailer by setting off.
	 */
	eh = (struct ether_header *)ifrw->ifrw_addr;
	eh->ether_type = ntohs((u_short)eh->ether_type);
#define	dedataaddr(eh, off, type)	((type)(((caddr_t)((eh)+1)+(off))))
	if (eh->ether_type >= ETHERTYPE_TRAIL &&
	    eh->ether_type < ETHERTYPE_TRAIL+ETHERTYPE_NTRAILER) {
		off = (eh->ether_type - ETHERTYPE_TRAIL) * 512;
		if (off >= ETHERMTU)
			return;		/* sanity */
		eh->ether_type = ntohs(*dedataaddr(eh, off, u_short *));
		resid = ntohs(*(dedataaddr(eh, off+2, u_short *)));
		if (off + resid > len)
			return;		/* sanity */
		len = off + resid;
	} else
		off = 0;
	if (len == 0)
		return;

	/*
	 * Pull packet off interface.  Off is nonzero if packet
	 * has trailing header; if_ubaget will then force this header
	 * information to be at the front, but we still have to drop
	 * the type and length which are at the front of any trailer data.
	 */
	m = if_ubaget(&ds->ds_deuba, ifrw, len, off, &ds->ds_if);
	if (m == 0)
		return;
	if (off) {
		struct ifnet *ifp;

		ifp = *(mtod(m, struct ifnet **));
		m->m_off += 2 * sizeof (u_short);
		m->m_len -= 2 * sizeof (u_short);
		*(mtod(m, struct ifnet **)) = ifp;
	}
	switch (eh->ether_type) {

#ifdef INET
	case ETHERTYPE_IP:
		schednetisr(NETISR_IP);
		inq = &ipintrq;
		break;

	case ETHERTYPE_ARP:
		arpinput(&ds->ds_ac, m);
		return;
#endif
#ifdef NS
	case ETHERTYPE_NS:
		schednetisr(NETISR_NS);
		inq = &nsintrq;
		break;

#endif
	default:
#if	DLI
	{
		eh->ether_type = htons(eh->ether_type);
		dli_input(m, eh->ether_type, &eh->ether_shost[0], 
 			  &de_dlv[ds->ds_if.if_unit], eh);
	}
#else	DLI
		m_freem(m);
#endif	DLI
		return;
	}

	s = splimp();
	if (IF_QFULL(inq)) {
		IF_DROP(inq);
		splx(s);
		m_freem(m);
		return;
	}
	IF_ENQUEUE(inq, m);
	splx(s);
#endif	MACH_KERNEL
}

#ifdef	MACH_KERNEL
deoutput(dev, ior)
	dev_t	dev;
	io_req_t ior;
{
	register int unit = minor(dev);
	if (unit >= NDE)
	    return (ENXIO);
	return (net_write(&de_softc[unit].ds_if, destart, ior));
}

desetinput(dev, receive_port, priority, filter, filter_count)
	dev_t		dev;
	mach_port_t	receive_port;
	int		priority;
	filter_t	filter[];
	unsigned int	filter_count;
{
	register int unit = minor(dev);
	if (unit >= NDE)
	    return (ENXIO);

	return (net_set_filter(&de_softc[unit].ds_if,
			receive_port, priority,
			filter, filter_count));
}

#else	MACH_KERNEL
/*
 * Ethernet output routine.
 * Encapsulate a packet of type family for the local net.
 * Use trailer local net encapsulation if enough data in first
 * packet leaves a multiple of 512 bytes of data in remainder.
 */
deoutput(ifp, m0, dst)
	struct ifnet *ifp;
	struct mbuf *m0;
	struct sockaddr *dst;
{
	int type, s, error;
 	u_char edst[6];
	struct in_addr idst;
	register struct de_softc *ds = &de_softc[ifp->if_unit];
	register struct mbuf *m = m0;
	register struct ether_header *eh;
	register int off;
	int usetrailers;

	if ((ifp->if_flags & (IFF_UP|IFF_RUNNING)) != (IFF_UP|IFF_RUNNING)) {
		error = ENETDOWN;
		goto bad;
	}
	switch (dst->sa_family) {

#ifdef INET
	case AF_INET:
		idst = ((struct sockaddr_in *)dst)->sin_addr;
 		if (!arpresolve(&ds->ds_ac, m, &idst, edst, &usetrailers))
			return (0);	/* if not yet resolved */
		off = ntohs((u_short)mtod(m, struct ip *)->ip_len) - m->m_len;
		if (usetrailers && off > 0 && (off & 0x1ff) == 0 &&
		    m->m_off >= MMINOFF + 2 * sizeof (u_short)) {
			type = ETHERTYPE_TRAIL + (off>>9);
			m->m_off -= 2 * sizeof (u_short);
			m->m_len += 2 * sizeof (u_short);
			*mtod(m, u_short *) = htons((u_short)ETHERTYPE_IP);
			*(mtod(m, u_short *) + 1) = htons((u_short)m->m_len);
			goto gottrailertype;
		}
		type = ETHERTYPE_IP;
		off = 0;
		goto gottype;
#endif
#ifdef NS
	case AF_NS:
		type = ETHERTYPE_NS;
 		bcopy((caddr_t)&(((struct sockaddr_ns *)dst)->sns_addr.x_host),
		(caddr_t)edst, sizeof (edst));
		off = 0;
		goto gottype;
#endif

#if	DLI
	case AF_DLI:
		if (m->m_len < sizeof(struct ether_header))
		{
			error = EMSGSIZE;
			goto bad;
		}
		eh = mtod(m, struct ether_header *);
 		bcopy(dst->sa_data, (caddr_t)eh->ether_dhost, sizeof (eh->ether_dhost));
		goto gotheader;
#endif	DLI

	case AF_UNSPEC:
		eh = (struct ether_header *)dst->sa_data;
 		bcopy((caddr_t)eh->ether_dhost, (caddr_t)edst, sizeof (edst));
		type = eh->ether_type;
		goto gottype;

	default:
		printf("de%d: can't handle af%d\n", ifp->if_unit,
			dst->sa_family);
		error = EAFNOSUPPORT;
		goto bad;
	}

gottrailertype:
	/*
	 * Packet to be sent as trailer: move first packet
	 * (control information) to end of chain.
	 */
	while (m->m_next)
		m = m->m_next;
	m->m_next = m0;
	m = m0->m_next;
	m0->m_next = 0;
	m0 = m;

gottype:
	/*
	 * Add local net header.  If no space in first mbuf,
	 * allocate another.
	 */
	if (m->m_off > MMAXOFF ||
	    MMINOFF + sizeof (struct ether_header) > m->m_off) {
		m = m_get(M_DONTWAIT, MT_HEADER);
		if (m == 0) {
			error = ENOBUFS;
			goto bad;
		}
		m->m_next = m0;
		m->m_off = MMINOFF;
		m->m_len = sizeof (struct ether_header);
	} else {
		m->m_off -= sizeof (struct ether_header);
		m->m_len += sizeof (struct ether_header);
	}
	eh = mtod(m, struct ether_header *);
	eh->ether_type = htons((u_short)type);
 	bcopy((caddr_t)edst, (caddr_t)eh->ether_dhost, sizeof (edst));
#if	DLI
gotheader:
#endif	DLI
	/* DEUNA fills in source address */

	/*
	 * Queue message on interface, and start output if interface
	 * not yet active.
	 */
	s = splimp();
	if (IF_QFULL(&ifp->if_snd)) {
		IF_DROP(&ifp->if_snd);
		splx(s);
		m_freem(m);
		return (ENOBUFS);
	}
	IF_ENQUEUE(&ifp->if_snd, m);
	destart(ifp->if_unit);
	splx(s);
	return (0);

bad:
	m_freem(m0);
	return (error);
}
#endif	MACH_KERNEL

#ifdef	MACH_KERNEL
degetstat(dev, flavor, status, count)
	dev_t	dev;
	int	flavor;
	dev_status_t	status;		/* pointer to OUT array */
	unsigned int	*count;		/* out */
{
	register int unit = minor(dev);
	register struct uba_device *ui;
	register struct de_softc *ds;

	if (unit >= NDE || (ui = deinfo[unit]) == 0 || ui->ui_alive == 0)
	    return (ENXIO);

	ds = &de_softc[unit];

	return (net_getstat(&ds->ds_if, flavor, status, count));
}

/*ARGSUSED*/
desetstat(dev, flavor, status, count)
	dev_t	dev;
	int	flavor;
	dev_status_t	status;
	unsigned int	count;
{
	register int unit = minor(dev);
	register struct uba_device *ui;
	register struct de_softc *ds;

	if (unit >= NDE || (ui = deinfo[unit]) == 0 || ui->ui_alive == 0)
		return (ENXIO);

	ds = &de_softc[unit];

	switch (flavor) {
	    case NET_STATUS:
	    {
		/*
		 * All we can change are flags, and not many of those.
		 */
		register struct net_status *ns = (struct net_status *)status;
		short mode = 0;

		if (count < NET_STATUS_COUNT)
		    return (D_INVALID_OPERATION);

		if (ns->flags & IFF_ALLMULTI)
		    mode |= MOD_ENAL;
		if (ns->flags & IFF_PROMISC)
		    mode |= MOD_PROM;

		/*
		 * Force a complete reset if the receive multicast or
		 * promiscuous mode changes, so that these take effect
		 * immediately.
		 */
		if (ds->ds_mode != mode) {
		    ds->ds_mode = mode;
		    if (ds->ds_flags & DSF_RUNNING) {
			((struct dedevice *)ui->ui_addr)->pclow = PCSR0_RSET;
			ds->ds_flags &= ~(DSF_LOCK | DSF_RUNNING);
		    }
		}
		ds->ds_if.if_flags = ns->flags & ~IFF_CANTCHANGE;

		if ((ds->ds_if.if_flags & IFF_UP) == 0 &&
			(ds->ds_flags & DSF_RUNNING)) {
		    ((struct dedevice *)ui->ui_addr)->pclow = 0;
		    DELAY(100);
		    ((struct dedevice *)ui->ui_addr)->pclow = PCSR0_RSET;
		    ds->ds_flags &= ~(DSF_LOCK | DSF_RUNNING);
		}
		else
		if ((ds->ds_if.if_flags & IFF_UP) &&
			(ds->ds_flags & DSF_RUNNING) == 0) {
		    deinit(unit);
		}
		break;
	    }
	    case NET_ADDRESS:
	    {
		register union ether_cvt {
		    char	addr[6];
		    int		lwd[2];
		} *ec = (union ether_cvt *)status;

		if (count < sizeof(*ec)/sizeof(int))
		    return (D_INVALID_SIZE);
		ec->lwd[0] = ntohl(ec->lwd[0]);
		ec->lwd[1] = ntohl(ec->lwd[1]);

		de_setaddr(ec->addr, unit);
		break;
	    }
	    default:
		return (D_INVALID_OPERATION);
	}
	return (D_SUCCESS);
}

#else	MACH_KERNEL
/*
 * Process an ioctl request.
 */
deioctl(ifp, cmd, data)
	register struct ifnet *ifp;
	int cmd;
	caddr_t data;
{
	register struct ifaddr *ifa = (struct ifaddr *)data;
	register struct de_softc *ds = &de_softc[ifp->if_unit];
	int s = splimp(), error = 0;

	switch (cmd) {

	case SIOCSIFADDR:
		ifp->if_flags |= IFF_UP;
		deinit(ifp->if_unit);

		switch (ifa->ifa_addr.sa_family) {
#ifdef INET
		case AF_INET:
			((struct arpcom *)ifp)->ac_ipaddr =
				IA_SIN(ifa)->sin_addr;
			arpwhohas((struct arpcom *)ifp, &IA_SIN(ifa)->sin_addr);
			break;
#endif
#ifdef NS
		case AF_NS:
		    {
			register struct ns_addr *ina = &(IA_SNS(ifa)->sns_addr);
			
			if (ns_nullhost(*ina))
				ina->x_host = *(union ns_host *)(ds->ds_addr);
			else
				de_setaddr(ina->x_host.c_host,ifp->if_unit);
			break;
		    }
#endif
		}
		break;

	case SIOCSIFFLAGS:
#if	MACH
	{
		short mode = 0;

		if (ifp->if_flags & IFF_ALLMULTI)
			mode |= MOD_ENAL;
		if (ifp->if_flags & IFF_PROMISC)
			mode |= MOD_PROM;
		/*
		 *  Force a complete reset if the receive multicast/promiscuous
		 *  mode changes so that these take effect immediately.
		 */
		if (ds->ds_mode != mode)
		{
			ds->ds_mode = mode;
			if (ds->ds_flags & DSF_RUNNING)
			{
			    ((struct dedevice *)
			       (deinfo[ifp->if_unit]->ui_addr))->pclow =
								    PCSR0_RSET;
			    ds->ds_flags &= ~(DSF_LOCK | DSF_RUNNING);
			}
		}
	}
#endif	MACH
		if ((ifp->if_flags & IFF_UP) == 0 &&
		    ds->ds_flags & DSF_RUNNING) {
			((struct dedevice *)
			   (deinfo[ifp->if_unit]->ui_addr))->pclow = 0;
			DELAY(100);
			((struct dedevice *)
			   (deinfo[ifp->if_unit]->ui_addr))->pclow = PCSR0_RSET;
			ds->ds_flags &= ~(DSF_LOCK | DSF_RUNNING);
		} else if (ifp->if_flags & IFF_UP &&
		    (ds->ds_flags & DSF_RUNNING) == 0)
			deinit(ifp->if_unit);
		break;

	default:
		error = EINVAL;
	}
	splx(s);
	return (error);
}
#endif	MACH_KERNEL

/*
 * set ethernet address for unit
 */
de_setaddr(physaddr, unit)
	u_char *physaddr;
	int unit;
{
	register struct de_softc *ds = &de_softc[unit];
	struct uba_device *ui = deinfo[unit];
	register struct dedevice *addr= (struct dedevice *)ui->ui_addr;
	
	if (! (ds->ds_flags & DSF_RUNNING))
		return;
		
	bcopy(physaddr, &ds->ds_pcbb.pcbb2, 6);
	ds->ds_pcbb.pcbb0 = FC_WTPHYAD;
	addr->pclow = PCSR0_INTE|CMD_GETCMD;
	if (dewait(ui, "address change") == 0) {
		ds->ds_flags |= DSF_SETADDR;
		bcopy(physaddr, ds->ds_addr, 6);
	}
}

/*
 * Await completion of the named function
 * and check for errors.
 */
dewait(ui, fn)
	register struct uba_device *ui;
	char *fn;
{
	register struct dedevice *addr = (struct dedevice *)ui->ui_addr;
	register csr0;

	while ((addr->pcsr0 & PCSR0_INTR) == 0)
		;
	csr0 = addr->pcsr0;
#if	MACH
#else	MACH
	addr->pchigh = csr0 >> 8;
#endif	MACH
	if (csr0 & PCSR0_PCEI)
		printf("de%d: %s failed, csr0=%b csr1=%b\n", 
		    ui->ui_unit, fn, csr0, PCSR0_BITS, 
		    addr->pcsr1, PCSR1_BITS);
#if	MACH
	if ((csr0 & PCSR0_PCEI) && (addr->pcsr1 & PCSR1_PCTO) == 0)
	{
	        register struct de_softc *ds = &de_softc[ui->ui_unit];

		printf("PCBB0: %x\nPCBB2: %x\nPCBB4: %x\nPCBB6: %x\n",
			ds->ds_pcbb.pcbb0,
			ds->ds_pcbb.pcbb2,
			ds->ds_pcbb.pcbb4,
			ds->ds_pcbb.pcbb6);
	}
	addr->pchigh = csr0 >> 8;
#endif	MACH
	return (csr0 & PCSR0_PCEI);
}
#endif
