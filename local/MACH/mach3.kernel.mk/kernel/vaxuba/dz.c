/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * Copyright (c) 1988 Carnegie-Mellon University
 * Copyright (c) 1987 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	dz.c,v $
 * Revision 2.3  91/05/08  13:30:27  dbg
 * 	Move br and cvec to memory.
 * 	[91/03/26            dbg]
 * 
 * Revision 2.2  90/06/02  15:09:34  rpd
 * 	Converted to new IPC.
 * 	[90/03/26  23:08:50  rpd]
 * 
 * Revision 2.1  89/08/03  16:40:58  rwd
 * Created.
 * 
 * 11-Oct-88  David Golub (dbg) at Carnegie-Mellon University
 *	Converted for MACH_KERNEL.
 *
 * 26-Jan-86  Avadis Tevanian (avie) at Carnegie-Mellon University
 *	Upgraded to 4.3.
 *
 * 10-May-85  Mike Accetta (mja) at Carnegie-Mellon University
 *	Upgraded to 4.2BSD.  Carried over changes below.
 *	[V1(1)]
 *
 * 28-Mar-83  Mike Accetta (mja) at Carnegie-Mellon University
 *	CS_TTYLOC:  Added setting of terminal location in dzopen()
 *	(V3.06h).
 *
 */
 
#ifdef	MACH_KERNEL
#else	MACH_KERNEL
#include <cs_ttyloc.h>
#endif	MACH_KERNEL
/*
 * Copyright (c) 1982, 1986 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)dz.c	7.1 (Berkeley) 6/5/86
 */

#include <dz.h>
#if NDZ > 0
/*
 *  DZ-11/DZ-32 Driver
 *
 * This driver mimics dh.c; see it for explanation of common code.
 */
#ifdef	MACH_KERNEL
#include <kern/time_out.h>
#include <sys/syslog.h>

#include <device/conf.h>
#include <device/tty.h>
#include <device/errno.h>

#include <vax/vm_defs.h>
#else	MACH_KERNEL
#include "bk.h"

#include "machine/pte.h"

#include "sys/param.h"
#include "sys/systm.h"
#include "sys/ioctl.h"
#include "sys/tty.h"
#include "sys/dir.h"
#include "sys/user.h"
#include "sys/proc.h"
#include "sys/map.h"
#include "sys/buf.h"
#include "sys/vm.h"
#include "sys/conf.h"
#include "sys/bkmac.h"
#include "sys/file.h"
#include "sys/uio.h"
#include "sys/kernel.h"
#include "sys/syslog.h"
#endif	MACH_KERNEL

#include <vaxuba/pdma.h>
#include <vaxuba/ubavar.h>
#include <vaxuba/dzreg.h>

/*
 * Driver information for auto-configuration stuff.
 */
int	dzprobe(), dzattach(), dzrint();
struct	uba_device *dzinfo[NDZ];
u_short	dzstd[] = { 0 };
struct	uba_driver dzdriver =
	{ dzprobe, 0, dzattach, 0, dzstd, "dz", dzinfo };

#define	NDZLINE 	(NDZ*8)
#define	FASTTIMER	(hz/30)		/* rate to drain silos, when in use */

int	dzstart(), dzxint(), dzdma();
#ifdef	MACH_KERNEL
int	dzstop();
#endif	MACH_KERNEL
int	ttrstrt();
struct	tty dz_tty[NDZLINE];
int	dz_cnt = { NDZLINE };
int	dzact;
int	dzsilos;			/* mask of dz's with silo in use */
int	dzchars[NDZ];			/* recent input count */
int	dzrate[NDZ];			/* smoothed input count */
int	dztimerintvl;			/* time interval for dztimer */
int	dzhighrate = 100;		/* silo on if dzchars > dzhighrate */
int	dzlowrate = 75;			/* silo off if dzrate < dzlowrate */

#define dzwait(x)	while (((x)->dzlcs & DZ_ACK) == 0)

/*
 * Software copy of dzbrk since it isn't readable
 */
char	dz_brk[NDZ];
char	dzsoftCAR[NDZ];
char	dz_lnen[NDZ];	/* saved line enable bits for DZ32 */

/*
 * The dz11 doesn't interrupt on carrier transitions, so
 * we have to use a timer to watch it.
 */
char	dz_timer;		/* timer started? */

/*
 * Pdma structures for fast output code
 */
struct	pdma dzpdma[NDZLINE];

char	dz_speeds[] =
	{ 0,020,021,022,023,024,0,025,026,027,030,032,034,036,037,0 };
 
#ifndef	PORTSELECTOR
#define	ISPEED	B9600
#define	IFLAGS	(EVENP|ODDP|ECHO)
#else
#define	ISPEED	B4800
#define	IFLAGS	(EVENP|ODDP)
#endif

extern volatile int	br, cvec;

dzprobe(reg)
	caddr_t reg;
{
	register struct dzdevice *dzaddr = (struct dzdevice *)reg;

#ifdef lint
	dzrint(0); dzxint((struct tty *)0);
#endif
	dzaddr->dzcsr = DZ_TIE|DZ_MSE|DZ_32;
	if (dzaddr->dzcsr & DZ_32)
		dzaddr->dzlnen = 1;
	else
		dzaddr->dztcr = 1;		/* enable any line */
	DELAY(100000);
	dzaddr->dzcsr = DZ_CLR|DZ_32;		/* reset everything */
	if (cvec && cvec != 0x200)
		cvec -= 4;
	return (sizeof (struct dzdevice));
}

dzattach(ui)
	register struct uba_device *ui;
{
	register struct pdma *pdp = &dzpdma[ui->ui_unit*8];
	register struct tty *tp = &dz_tty[ui->ui_unit*8];
	register int cntr;
	extern dzscan();

	for (cntr = 0; cntr < 8; cntr++) {
		pdp->p_addr = (struct dzdevice *)ui->ui_addr;
		pdp->p_arg = (int)tp;
		pdp->p_fcn = dzxint;
		pdp++, tp++;
	}
	dzsoftCAR[ui->ui_unit] = ui->ui_flags;
	if (dz_timer == 0) {
		dz_timer++;
		timeout(dzscan, (caddr_t)0, hz);
		dztimerintvl = FASTTIMER;
	}
}

/*ARGSUSED*/
#ifdef	MACH_KERNEL
dzopen(dev, flag, ior)
	dev_t	dev;
	int	flag;
	io_req_t ior;
#else	MACH_KERNEL
dzopen(dev, flag)
	dev_t dev;
#endif	MACH_KERNEL
{
	register struct tty *tp;
	register int unit;
#ifdef	MACH_KERNEL
#else	MACH_KERNEL
#if	CS_TTYLOC
	static tlcinit = 0;

	/*
	 *  Initialize terminal locations on first call to driver.
	 */
	if (tlcinit == 0)
	{
		tlcinit++;
		for (unit=0; unit<dz_cnt; unit++)
		{
			tp = &dz_tty[unit];
			tp->t_ttyloc.tlc_hostid = TLC_MYHOST;
			tp->t_ttyloc.tlc_ttyid = unit;
		}
	}
#endif	CS_TTYLOC
#endif	MACH_KERNEL
 
	unit = minor(dev);
	if (unit >= dz_cnt || dzpdma[unit].p_addr == 0)
		return (ENXIO);
	tp = &dz_tty[unit];
	tp->t_addr = (caddr_t)&dzpdma[unit];
	tp->t_oproc = dzstart;
#ifdef	MACH_KERNEL
	tp->t_stop = dzstop;
#endif	MACH_KERNEL
	if ((tp->t_state & TS_ISOPEN) == 0) {
		ttychars(tp);
#ifndef PORTSELECTOR
		if (tp->t_ispeed == 0) {
#else
			tp->t_state |= TS_HUPCLS;
#endif PORTSELECTOR
			tp->t_ispeed = ISPEED;
			tp->t_ospeed = ISPEED;
			tp->t_flags = IFLAGS;
#ifndef PORTSELECTOR
		}
#endif PORTSELECTOR
		dzparam(unit);
	}
#ifdef	MACH_KERNEL
#else	MACH_KERNEL
	else if (tp->t_state&TS_XCLUDE && u.u_uid != 0)
		return (EBUSY);
#endif	MACH_KERNEL
	(void) dzmctl(dev, DZ_ON, DMSET);
#ifdef	MACH_KERNEL
	return (char_open(dev, tp, flag, ior));
#else	MACH_KERNEL
	(void) spl5();
	while ((tp->t_state & TS_CARR_ON) == 0) {
		tp->t_state |= TS_WOPEN;
		sleep((caddr_t)&tp->t_rawq, TTIPRI);
	}
	(void) spl0();
	return ((*linesw[tp->t_line].l_open)(dev, tp));
#endif MACH_KERNEL
}
 
/*ARGSUSED*/
dzclose(dev, flag)
	dev_t dev;
{
	register struct tty *tp;
	register int unit;
	register struct dzdevice *dzaddr;
	int dz;
#ifdef	MACH_KERNEL
	int s;
#endif	MACH_KERNEL
 
	unit = minor(dev);
	dz = unit >> 3;
	tp = &dz_tty[unit];
#ifdef	MACH_KERNEL
	s = spltty();
	simple_lock(&tp->t_lock);
#else	MACH_KERNEL
	(*linesw[tp->t_line].l_close)(tp);
#endif	MACH_KERNEL
	dzaddr = dzpdma[unit].p_addr;
	if (dzaddr->dzcsr&DZ_32)
		(void) dzmctl(dev, DZ_BRK, DMBIC);
	else
		dzaddr->dzbrk = (dz_brk[dz] &= ~(1 << (unit&07)));
	if ((tp->t_state&(TS_HUPCLS|TS_WOPEN)) || (tp->t_state&TS_ISOPEN) == 0)
		(void) dzmctl(dev, DZ_OFF, DMSET);
	ttyclose(tp);
#ifdef	MACH_KERNEL
	simple_unlock(&tp->t_lock);
	splx(s);
#endif	MACH_KERNEL
}
 
dzread(dev, uio)
	dev_t dev;
	struct uio *uio;
{
	register struct tty *tp;
 
	tp = &dz_tty[minor(dev)];
	return ((*linesw[tp->t_line].l_read)(tp, uio));
}
 
dzwrite(dev, uio)
	dev_t dev;
	struct uio *uio;
{
	register struct tty *tp;
 
	tp = &dz_tty[minor(dev)];
	return ((*linesw[tp->t_line].l_write)(tp, uio));
}

/*ARGSUSED*/
dzrint(dz)
	int dz;
{
	register struct tty *tp;
	register int c;
	register struct dzdevice *dzaddr;
	register struct tty *tp0;
	register int unit;
	int overrun = 0;
 
	if ((dzact & (1<<dz)) == 0)
		return;
	unit = dz * 8;
	dzaddr = dzpdma[unit].p_addr;
	tp0 = &dz_tty[unit];
	dzaddr->dzcsr &= ~(DZ_RIE|DZ_MIE);	/* the manual says this song */
	dzaddr->dzcsr |= DZ_RIE|DZ_MIE;		/*   and dance is necessary */
	while (dzaddr->dzcsr & DZ_MSC) {	/* DZ32 modem change interrupt */
		c = dzaddr->dzmtsr;
		tp = tp0 + (c&7);
		if (tp >= &dz_tty[dz_cnt])
			break;
		dzaddr->dzlcs = c&7;	/* get status of modem lines */
		dzwait(dzaddr);		/* wait for them */
		if (c & DZ_CD)		/* carrier status change? */
		if (dzaddr->dzlcs & DZ_CD) {	/* carrier up? */
			(void)(*linesw[tp->t_line].l_modem)(tp, 1);
		}
		else if ((*linesw[tp->t_line].l_modem)(tp, 0) == 0)
			dzaddr->dzlcs = DZ_ACK|(c&7);
	}
	while ((c = dzaddr->dzrbuf) < 0) {	/* char present */
		dzchars[dz]++;
		tp = tp0 + ((c>>8)&07);
		if (tp >= &dz_tty[dz_cnt])
			continue;
		if ((tp->t_state & TS_ISOPEN) == 0) {
#ifdef	MACH_KERNEL
			tt_open_wakeup(tp);
#else	MACH_KERNEL
			wakeup((caddr_t)&tp->t_rawq);
#endif	MACH_KERNEL
#ifdef PORTSELECTOR
			if ((tp->t_state&TS_WOPEN) == 0)
#endif
				continue;
		}
		if (c&DZ_FE)
#ifdef	MACH_KERNEL
			c = tp->t_breakc;
#else	MACH_KERNEL
			if (tp->t_flags & RAW)
				c = 0;
			else
				c = tp->t_intrc;
#endif	MACH_KERNEL
		if (c&DZ_DO && overrun == 0) {
			log(LOG_WARNING, "dz%d,%d: silo overflow\n", dz, (c>>8)&7);
			overrun = 1;
		}
		if (c&DZ_PE)	
			if (((tp->t_flags & (EVENP|ODDP)) == EVENP)
			  || ((tp->t_flags & (EVENP|ODDP)) == ODDP))
				continue;
#if NBK > 0
		if (tp->t_line == NETLDISC) {
			c &= 0177;
			BKINPUT(c, tp);
		} else
#endif
			(*linesw[tp->t_line].l_rint)(c, tp);
	}
}
 
#ifdef	MACH_KERNEL
int
dzportdeath(dev, port)
	dev_t	dev;
	mach_port_t port;
{
	return (tty_portdeath(&dz_tty[minor(dev)], port));
}

io_return_t dzgetstat(dev, flavor, data, count)
	dev_t		dev;
	int		flavor;
	int *		data;		/* pointer to OUT array */
	unsigned int	*count;		/* out */
{
	register struct tty *tp;
	register int	unit = minor(dev);

	tp = &dz_tty[unit];

	switch (flavor) {
	    case TTY_MODEM:
		*data = dztodm(dzmctl(dev, 0, DMGET));
		*count = 1;
		break;
	    default:
		return (tty_get_status(tp, flavor, data, count));
	}
	return (D_SUCCESS);
}

io_return_t dzsetstat(dev, flavor, data, count)
	dev_t		dev;
	int		flavor;
	int *		data;
	unsigned int	count;
{
	register struct tty *tp;
	register int	unit = minor(dev);
	register int	dz = unit >> 3;
	register struct dzdevice *	dzaddr;

	tp = &dz_tty[unit];

	switch (flavor) {
	    case TTY_MODEM:
		if (count < TTY_MODEM_COUNT)
		    return (D_INVALID_OPERATION);
		(void) dzmctl(dev, dmtodz(*data), DMSET);
		break;

	    case TTY_SET_BREAK:
		dzaddr = ((struct pdma *)(tp->t_addr))->p_addr;
		if (dzaddr->dzcsr&DZ_32)
		    (void) dzmctl(dev, DZ_BRK, DMBIS);
		else
		    dzaddr->dzbrk = (dz_brk[dz] |= 1 << (unit&07));
		break;

	    case TTY_CLEAR_BREAK:
		dzaddr = ((struct pdma *)(tp->t_addr))->p_addr;
		if (dzaddr->dzcsr&DZ_32)
		    (void) dzmctl(dev, DZ_BRK, DMBIC);
		else
		    dzaddr->dzbrk = (dz_brk[dz] &= ~(1 << (unit&07)));
		break;

	    case TTY_STATUS:
	    {
		register int error;
		error = tty_set_status(tp, flavor, data, count);
		if (error == 0)
		    dzparam(unit);
		return (error);
	    }
	    default:
		return (tty_set_status(tp, flavor, data, count));
	}
	return (D_SUCCESS);
}
#else	MACH_KERNEL
/*ARGSUSED*/
dzioctl(dev, cmd, data, flag)
	dev_t dev;
	caddr_t data;
{
	register struct tty *tp;
	register int unit = minor(dev);
	register int dz = unit >> 3;
	register struct dzdevice *dzaddr;
	int error;
 
	tp = &dz_tty[unit];
	error = (*linesw[tp->t_line].l_ioctl)(tp, cmd, data, flag);
	if (error >= 0)
		return (error);
	error = ttioctl(tp, cmd, data, flag);
	if (error >= 0) {
		if (cmd == TIOCSETP || cmd == TIOCSETN || cmd == TIOCLBIS ||
		    cmd == TIOCLBIC || cmd == TIOCLSET)
			dzparam(unit);
		return (error);
	}
	switch (cmd) {

	case TIOCSBRK:
		dzaddr = ((struct pdma *)(tp->t_addr))->p_addr;
		if (dzaddr->dzcsr&DZ_32)
			(void) dzmctl(dev, DZ_BRK, DMBIS);
		else
			dzaddr->dzbrk = (dz_brk[dz] |= 1 << (unit&07));
		break;

	case TIOCCBRK:
		dzaddr = ((struct pdma *)(tp->t_addr))->p_addr;
		if (dzaddr->dzcsr&DZ_32)
			(void) dzmctl(dev, DZ_BRK, DMBIC);
		else
			dzaddr->dzbrk = (dz_brk[dz] &= ~(1 << (unit&07)));
		break;

	case TIOCSDTR:
		(void) dzmctl(dev, DZ_DTR|DZ_RTS, DMBIS);
		break;

	case TIOCCDTR:
		(void) dzmctl(dev, DZ_DTR|DZ_RTS, DMBIC);
		break;

	case TIOCMSET:
		(void) dzmctl(dev, dmtodz(*(int *)data), DMSET);
		break;

	case TIOCMBIS:
		(void) dzmctl(dev, dmtodz(*(int *)data), DMBIS);
		break;

	case TIOCMBIC:
		(void) dzmctl(dev, dmtodz(*(int *)data), DMBIC);
		break;

	case TIOCMGET:
		*(int *)data = dztodm(dzmctl(dev, 0, DMGET));
		break;

	default:
		return (ENOTTY);
	}
	return (0);
}
#endif	MACH_KERNEL

dmtodz(bits)
	register int bits;
{
	register int b;

	b = (bits >>1) & 0370;
	if (bits & DML_ST) b |= DZ_ST;
	if (bits & DML_RTS) b |= DZ_RTS;
	if (bits & DML_DTR) b |= DZ_DTR;
	if (bits & DML_LE) b |= DZ_LE;
	return(b);
}

dztodm(bits)
	register int bits;
{
	register int b;

	b = (bits << 1) & 0360;
	if (bits & DZ_DSR) b |= DML_DSR;
	if (bits & DZ_DTR) b |= DML_DTR;
	if (bits & DZ_ST) b |= DML_ST;
	if (bits & DZ_RTS) b |= DML_RTS;
	return(b);
}
 
dzparam(unit)
	register int unit;
{
	register struct tty *tp;
	register struct dzdevice *dzaddr;
	register int lpr;
 
	tp = &dz_tty[unit];
	dzaddr = dzpdma[unit].p_addr;
	if (dzsilos & (1 << (unit >> 3)))
		dzaddr->dzcsr = DZ_IEN | DZ_SAE;
	else
		dzaddr->dzcsr = DZ_IEN;
	dzact |= (1<<(unit>>3));
	if (tp->t_ispeed == 0) {
		(void) dzmctl(unit, DZ_OFF, DMSET);	/* hang up line */
		return;
	}
	lpr = (dz_speeds[tp->t_ispeed]<<8) | (unit & 07);
	if (tp->t_flags & (RAW|LITOUT|PASS8))
		lpr |= BITS8;
	else
		lpr |= (BITS7|PENABLE);
	if ((tp->t_flags & EVENP) == 0)
		lpr |= OPAR;
	if (tp->t_ispeed == B110)
		lpr |= TWOSB;
	dzaddr->dzlpr = lpr;
}
 
dzxint(tp)
	register struct tty *tp;
{
	register struct pdma *dp;
	register dz, unit;
 
	dp = (struct pdma *)tp->t_addr;
	tp->t_state &= ~TS_BUSY;
	if (tp->t_state & TS_FLUSH)
		tp->t_state &= ~TS_FLUSH;
	else {
		ndflush(&tp->t_outq, dp->p_mem-tp->t_outq.c_cf);
		dp->p_end = dp->p_mem = tp->t_outq.c_cf;
	}
	if (tp->t_line)
		(*linesw[tp->t_line].l_start)(tp);
	else
		dzstart(tp);
	dz = minor(tp->t_dev) >> 3;
	unit = minor(tp->t_dev) & 7;
	if (tp->t_outq.c_cc == 0 || (tp->t_state&TS_BUSY)==0)
		if (dp->p_addr->dzcsr & DZ_32)
			dp->p_addr->dzlnen = (dz_lnen[dz] &= ~(1<<unit));
		else
			dp->p_addr->dztcr &= ~(1<<unit);
}

dzstart(tp)
	register struct tty *tp;
{
	register struct pdma *dp;
	register struct dzdevice *dzaddr;
	register int cc;
	int s, dz, unit;
 
	dp = (struct pdma *)tp->t_addr;
	dzaddr = dp->p_addr;
	s = spl5();
	if (tp->t_state & (TS_TIMEOUT|TS_BUSY|TS_TTSTOP))
		goto out;
	if (tp->t_outq.c_cc <= TTLOWAT(tp)) {
#ifdef	MACH_KERNEL
		tt_write_wakeup(tp);
#else	MACH_KERNEL
		if (tp->t_state&TS_ASLEEP) {
			tp->t_state &= ~TS_ASLEEP;
			wakeup((caddr_t)&tp->t_outq);
		}
		if (tp->t_wsel) {
			selwakeup(tp->t_wsel, tp->t_state & TS_WCOLL);
			tp->t_wsel = 0;
			tp->t_state &= ~TS_WCOLL;
		}
#endif	MACH_KERNEL
	}
	if (tp->t_outq.c_cc == 0)
		goto out;
	if (tp->t_flags & (RAW|LITOUT))
		cc = ndqb(&tp->t_outq, 0);
	else {
		cc = ndqb(&tp->t_outq, 0200);
		if (cc == 0) {
			cc = getc(&tp->t_outq);
			timeout(ttrstrt, (caddr_t)tp, (cc&0x7f) + 6);
			tp->t_state |= TS_TIMEOUT;
			goto out;
		}
	}
	tp->t_state |= TS_BUSY;
	dp->p_end = dp->p_mem = tp->t_outq.c_cf;
	dp->p_end += cc;
	dz = minor(tp->t_dev) >> 3;
	unit = minor(tp->t_dev) & 7;
	if (dzaddr->dzcsr & DZ_32)
		dzaddr->dzlnen = (dz_lnen[dz] |= (1<<unit));
	else
		dzaddr->dztcr |= (1<<unit);
out:
	splx(s);
}
 
/*
 * Stop output on a line.
 */
/*ARGSUSED*/
dzstop(tp, flag)
	register struct tty *tp;
{
	register struct pdma *dp;
	register int s;

	dp = (struct pdma *)tp->t_addr;
	s = spl5();
	if (tp->t_state & TS_BUSY) {
		dp->p_end = dp->p_mem;
		if ((tp->t_state&TS_TTSTOP)==0)
			tp->t_state |= TS_FLUSH;
	}
	splx(s);
}
 
dzmctl(dev, bits, how)
	dev_t dev;
	int bits, how;
{
	register struct dzdevice *dzaddr;
	register int unit, mbits;
	int b, s;

	unit = minor(dev);
	b = 1<<(unit&7);
	dzaddr = dzpdma[unit].p_addr;
	s = spl5();
	if (dzaddr->dzcsr & DZ_32) {
		dzwait(dzaddr)
		DELAY(100);		/* IS 100 TOO MUCH? */
		dzaddr->dzlcs = unit&7;
		DELAY(100);
		dzwait(dzaddr)
		DELAY(100);
		mbits = dzaddr->dzlcs;
		mbits &= 0177770;
	} else {
		mbits = (dzaddr->dzdtr & b) ? DZ_DTR : 0;
		mbits |= (dzaddr->dzmsr & b) ? DZ_CD : 0;
		mbits |= (dzaddr->dztbuf & b) ? DZ_RI : 0;
	}
	switch (how) {
	case DMSET:
		mbits = bits;
		break;

	case DMBIS:
		mbits |= bits;
		break;

	case DMBIC:
		mbits &= ~bits;
		break;

	case DMGET:
		(void) splx(s);
		return(mbits);
	}
	if (dzaddr->dzcsr & DZ_32) {
		mbits |= DZ_ACK|(unit&7);
		dzaddr->dzlcs = mbits;
	} else {
		if (mbits & DZ_DTR)
			dzaddr->dzdtr |= b;
		else
			dzaddr->dzdtr &= ~b;
	}
	(void) splx(s);
	return(mbits);
}
 
int dztransitions, dzfasttimers;		/*DEBUG*/
dzscan()
{
	register i;
	register struct dzdevice *dzaddr;
	register bit;
	register struct tty *tp;
	register car;
	int olddzsilos = dzsilos;
	int dztimer();
 
	for (i = 0; i < dz_cnt ; i++) {
		dzaddr = dzpdma[i].p_addr;
		if (dzaddr == 0)
			continue;
		tp = &dz_tty[i];
		bit = 1<<(i&07);
		car = 0;
		if (dzsoftCAR[i>>3]&bit)
			car = 1;
		else if (dzaddr->dzcsr & DZ_32) {
			dzaddr->dzlcs = i&07;
			dzwait(dzaddr);
			car = dzaddr->dzlcs & DZ_CD;
		} else
			car = dzaddr->dzmsr&bit;
		if (car) {
			/* carrier present */
			if ((tp->t_state & TS_CARR_ON) == 0)
				(void)(*linesw[tp->t_line].l_modem)(tp, 1);
		} else if ((tp->t_state&TS_CARR_ON) &&
		    (*linesw[tp->t_line].l_modem)(tp, 0) == 0)
			dzaddr->dzdtr &= ~bit;
	}
	for (i = 0; i < NDZ; i++) {
		ave(dzrate[i], dzchars[i], 8);
		if (dzchars[i] > dzhighrate && ((dzsilos & (1 << i)) == 0)) {
			dzpdma[i << 3].p_addr->dzcsr = DZ_IEN | DZ_SAE;
			dzsilos |= (1 << i);
			dztransitions++;		/*DEBUG*/
		} else if ((dzsilos & (1 << i)) && (dzrate[i] < dzlowrate)) {
			dzpdma[i << 3].p_addr->dzcsr = DZ_IEN;
			dzsilos &= ~(1 << i);
		}
		dzchars[i] = 0;
	}
	if (dzsilos && !olddzsilos)
		timeout(dztimer, (caddr_t)0, dztimerintvl);
	timeout(dzscan, (caddr_t)0, hz);
}

dztimer()
{
	register int dz;
	register int s;

	if (dzsilos == 0)
		return;
	s = spl5();
	dzfasttimers++;		/*DEBUG*/
	for (dz = 0; dz < NDZ; dz++)
		if (dzsilos & (1 << dz))
		    dzrint(dz);
	splx(s);
	timeout(dztimer, (caddr_t) 0, dztimerintvl);
}

/*
 * Reset state of driver if UBA reset was necessary.
 * Reset parameters and restart transmission on open lines.
 */
dzreset(uban)
	int uban;
{
	register int unit;
	register struct tty *tp;
	register struct uba_device *ui;

	for (unit = 0; unit < NDZLINE; unit++) {
		ui = dzinfo[unit >> 3];
		if (ui == 0 || ui->ui_ubanum != uban || ui->ui_alive == 0)
			continue;
		if (unit%8 == 0)
			printf(" dz%d", unit>>3);
		tp = &dz_tty[unit];
		if (tp->t_state & (TS_ISOPEN|TS_WOPEN)) {
			dzparam(unit);
			(void) dzmctl(unit, DZ_ON, DMSET);
			tp->t_state &= ~TS_BUSY;
			dzstart(tp);
		}
	}
}
#endif
