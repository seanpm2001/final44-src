/* 
 * Mach Operating System
 * Copyright (c) 1994-1989 Carnegie Mellon University
 * All Rights Reserved.
 * 
 * Permission to use, copy, modify and distribute this software and its
 * documentation is hereby granted, provided that both the copyright
 * notice and this permission notice appear in all copies of the
 * software, derivative works or modified versions, and any portions
 * thereof, and that both notices appear in supporting documentation.
 * 
 * CARNEGIE MELLON ALLOWS FREE USE OF THIS SOFTWARE IN ITS "AS IS"
 * CONDITION.  CARNEGIE MELLON DISCLAIMS ANY LIABILITY OF ANY KIND FOR
 * ANY DAMAGES WHATSOEVER RESULTING FROM THE USE OF THIS SOFTWARE.
 * 
 * Carnegie Mellon requests users of this software to return to
 * 
 *  Software Distribution Coordinator  or  Software.Distribution@CS.CMU.EDU
 *  School of Computer Science
 *  Carnegie Mellon University
 *  Pittsburgh PA 15213-3890
 * 
 * any improvements or extensions that they make and grant Carnegie Mellon
 * the rights to redistribute these changes.
 */
/* 
 * HISTORY
 * $Log:	fd.c,v $
 * Revision 2.17  94/12/15  15:39:00  dbg
 * 	Combine bus_device and bus_ctlr structures.
 * 	[94/07/20            dbg]
 * 
 * 	Renamed fddriver to fdcdriver to match standard IOCONF.
 * 	[94/06/16            dbg]
 * 
 * 	Include device/pc_disk.h instead of i386at/disk.h for
 * 	disk get/set status definitions.
 * 	[94/02/21            dbg]
 * 
 * 	ANSI-fied.
 * 	[93/12/28            dbg]
 * 
 * 	Missed one debug printf
 * 	[93/12/06            rvb]
 * 
 * 	Use special page for dma to addresses > FdDmaThreshold!
 * 	[93/11/29            rvb]
 * 
 * Revision 2.16  93/11/17  16:44:25  dbg
 * 	Changed HZ to hz.  Added include of kern/time_out.h.
 * 	Removed non-MACH_KERNEL code.
 * 	[93/05/21            dbg]
 * 
 * Revision 2.15  93/08/02  21:43:43  mrt
 * 	Put the free copyright on this file, again.
 * 	[93/07/22            mrt]
 * 
 * Revision 2.14  93/05/15  19:32:35  mrt
 * 	machparam.h -> machspl.h
 * 
 * Revision 2.13  93/05/10  21:19:02  rvb
 * 	Lint.
 * 	[93/05/08  11:19:03  af]
 * 
 * Revision 2.12  93/01/24  13:15:28  danner
 * 	[93/01/19  11:03:43  rvb]
 * 
 *	Test lower bound of media type; convert fd_addr to vm_offset_t.
 * 
 * Revision 2.11  93/01/14  17:30:11  danner
 * 	Proper spl typing.
 * 	[92/11/30            af]
 * 
 * Revision 2.10  92/07/09  22:53:34  rvb
 * 	There is a problem with ADAPTEC boards, where scsi disk dma will
 * 	lock out the floppy dma.  An expedient fix seems to be to drop
 * 	the DMA size for the floppy, in the retry code when/if this happens.
 * 	[92/07/08            rvb]
 * 
 * Revision 2.9  92/04/08  08:58:30  rpd
 * 	Fixed fdprobe to loop longer.  From rvb.
 * 	[92/04/08            rpd]
 * 
 * Revision 2.8  92/02/23  22:42:57  elf
 * 	Added (mandatory) DEV_GET_SIZE flavor of get_status.
 * 	[92/02/22            af]
 * 
 * Revision 2.7  92/01/27  16:42:54  rpd
 * 	Fixed fdgetstat and fdsetstat to return D_INVALID_OPERATION
 * 	for unsupported flavors.
 * 	[92/01/26            rpd]
 * 
 * Revision 2.6  91/11/12  11:09:18  rvb
 * 	Amazing how hard getting the probe to work for all machines is.
 * 	[91/10/16            rvb]
 * 
 * Revision 2.5  91/10/07  17:25:22  af
 * 	Still better
 * 	[91/10/07  16:29:57  rvb]
 * 
 * 	From mg32: Better probe for multiple controllers now possible.
 * 	[91/09/23            rvb]
 * 	New chips/busses.[ch] nomenclature.
 * 	[91/09/09  17:12:23  rvb]
 * 
 * 	Added a reset in open to prevent "no such device" errors
 * 	Added dlb's fddevinfo.
 * 	Reworked to make 2.5/3.0 compatible
 * 	[91/09/04  15:46:49  rvb]
 * 
 *	Major rewrite by mg32.
 * 	[91/08/07            mg32]
 * 
 * Revision 2.4  91/08/24  11:57:32  af
 * 	New MI autoconf.
 * 	[91/08/02  02:53:26  af]
 * 
 * Revision 2.3  91/05/14  16:22:47  mrt
 * 	Correcting copyright
 * 
 * Revision 2.2  91/02/14  14:42:23  mrt
 * 	This file is the logical concatenation of the previous c765.c,
 * 	m765knl.c and m765drv.c, in that order.  
 * 	[91/01/15            rvb]
 * 
 * Revision 2.5  91/01/08  17:33:32  rpd
 * 	Add some 3.0 get/set stat stuff.
 * 	[91/01/04  12:21:06  rvb]
 * 
 * Revision 2.4  90/11/26  14:50:54  rvb
 * 	jsb beat me to XMK34, sigh ...
 * 	[90/11/26            rvb]
 * 	Synched 2.5 & 3.0 at I386q (r1.6.1.6) & XMK35 (r2.4)
 * 	[90/11/15            rvb]
 * 
 * Revision 2.3  90/08/27  22:01:22  dbg
 * 	Remove include of device/param.h (unnecessary).  Flush ushort.
 * 	[90/07/17            dbg]
 * 
 * Revision 1.6.1.5  90/08/25  15:44:31  rvb
 * 	Use take_<>_irq() vs direct manipulations of ivect and friends.
 * 	[90/08/20            rvb]
 * 
 * Revision 1.6.1.4  90/07/27  11:26:53  rvb
 * 	Fix Intel Copyright as per B. Davies authorization.
 * 	[90/07/27            rvb]
 * 
 * Revision 1.6.1.3  90/07/10  11:45:11  rvb
 * 	New style probe/attach.
 * 	NOTE: the whole probe/slave/attach is a crock.  Someone
 * 	who spends the time to understand the driver should do
 * 	it right.
 * 	[90/06/15            rvb]
 * 
 * Revision 2.2  90/05/03  15:45:37  dbg
 * 	Convert for pure kernel.
 *	Optimized fd_disksort iff dp empty.
 * 	[90/04/19            dbg]
 * 
 * Revision 1.6.1.2  90/01/08  13:30:14  rvb
 * 	Add Intel copyright.
 * 	[90/01/08            rvb]
 * 
 * Revision 1.6.1.1  89/10/22  11:34:51  rvb
 * 	Received from Intel October 5, 1989.
 * 	[89/10/13            rvb]
 * 
 * Revision 1.6  89/09/25  12:27:05  rvb
 * 	vtoc.h -> disk.h
 * 	[89/09/23            rvb]
 * 
 * Revision 1.5  89/09/09  15:23:15  rvb
 * 	Have fd{read,write} use stragegy now that physio maps correctly.
 * 	[89/09/06            rvb]
 * 
 * Revision 1.4  89/03/09  20:07:26  rpd
 * 	More cleanup.
 * 
 * Revision 1.3  89/02/26  12:40:28  gm0w
 * 	Changes for cleanup.
 *
 */
 
/*
  Copyright 1988, 1989 by Intel Corporation, Santa Clara, California.

		All Rights Reserved

Permission to use, copy, modify, and distribute this software and
its documentation for any purpose and without fee is hereby
granted, provided that the above copyright notice appears in all
copies and that both the copyright notice and this permission notice
appear in supporting documentation, and that the name of Intel
not be used in advertising or publicity pertaining to distribution
of the software without specific, written prior permission.

INTEL DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE
INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS,
IN NO EVENT SHALL INTEL BE LIABLE FOR ANY SPECIAL, INDIRECT, OR
CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
LOSS OF USE, DATA OR PROFITS, WHETHER IN ACTION OF CONTRACT,
NEGLIGENCE, OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION
WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

/*	Copyright (c) 1987, 1988 TOSHIBA Corp.		*/
/*		All Rights Reserved			*/

#include <fd.h>

#include <kern/kern_io.h>
#include <kern/memory.h>
#include <kern/sched_prim.h>
#include <kern/time_out.h>

#include <sys/types.h>
#include <sys/ioctl.h>

#include <device/blkio.h>
#include <device/buf.h>
#include <device/errno.h>
#include <device/pc_disk.h>

#include <i386/pio.h>
#include <i386/machspl.h>
#include <chips/busses.h>
#include <i386at/fdreg.h>
#include <vm/vm_kern.h>

#ifdef	DEBUG
#define D(x) x
#define DD(x) x
#else	/* DEBUG */
#define D(x)
#define DD(x)
#endif	/* DEBUG */

/*
 * Floppy Device-Table Definitions (drtabs)
 *
 *      Cyls,Sec,spc,part,Mtype,RWFpl,FGpl
 */
struct	fddrtab m765f[] = {			/* format table */
      {	80, 18, 1440,  9, 0x88, 0x2a, 0x50 },	/* [0] 3.50" 720  Kb  */
      {	80, 36, 2880, 18, 0x08, 0x1b, 0x6c },	/* [1] 3.50" 1.44 Meg */
      {	40, 18,  720,  9, 0xa8, 0x2a, 0x50 },	/* [2] 5.25" 360  Kb  */
      {	80, 30, 2400, 15, 0x08, 0x1b, 0x54 }	/* [3] 5.25" 1.20 Meg */
};

/*
 * The following are static initialization variables
 * which are based on the configuration.
 */
struct ctrl_info ctrl_info[MAXUNIT>>1] = {		/* device data table */
	{  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } ,
	{  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

struct unit_info unit_info[MAXUNIT];		/* unit buffer headers	*/

char *fderr = "FD Error on unit";
char *fdmsg[] = {
	"?",
	"Missing data address mark",
	"Write protected",
	"Sector not found",
	"Data Overrun",				/* Over run error */
	"Uncorrectable data read error",	/* CRC Error */
	"FDC Error",
	"Illegal format type",
	"Drive not ready",
	"diskette not present - please insert",
	"Illegal interrupt type"
};

boolean_t fdprobe(vm_offset_t, struct bus_device *);
int	fdslave(struct bus_device *, vm_offset_t);
void	fdattach(struct bus_device *);
void	fdintr(int);

int	FdDmaEISA = 0;
int	FdDmaThreshold = 16 * 1024 * 1024;
vm_offset_t	FdDmaPage = (vm_offset_t) 0;

vm_offset_t	fd_std[] = { 0 };
struct	bus_device *fd_dinfo[NFD*2];
struct	bus_device *fd_minfo[NFD];
struct	bus_driver	fdcdriver = 
	{fdprobe, fdslave, fdattach, 0, fd_std,
	 "fd", fd_dinfo, "fdc", fd_minfo, 0};

int	m765verify[MAXUNIT] = {1,1,1,1};	/* write after read flag */
						/* 0 != verify mode	*/ 
						/* 0 == not verify mode */
/*
 *	Forward declarations
 */
void openchk(
	struct	fdcmd	*cmdp);
void chkbusy(
	struct	fdcmd	*cmdp);
void openfre(
	struct	fdcmd	*cmdp);
void setqueue(
	struct buf *bp,
	struct unit_info *uip);
void fd_disksort(
	struct unit_info *uip,
	struct buf *bp);
void quechk(
	struct unit_info *uip);
void m765io(
	struct unit_info *uip);
void m765iosub(
	struct unit_info *uip);
void rwcmdset(
	struct unit_info *uip);
int outicmd(
	struct unit_info *uip);
void m765intrsub(
	struct unit_info *uip);
void rwintr(
	struct unit_info *uip);
void rwierr(
	struct unit_info *uip);
void rbintr(
	struct unit_info *uip);
void seekintr(
	struct unit_info *uip);
void seekintre(
	struct unit_info *uip);
void seekierr(
	struct unit_info *uip,
	register char 	seekpoint);
void intrerr0(
	struct unit_info *uip);
void rstout(
	struct unit_info *uip);
void specify(
	struct unit_info *uip);
int rbrate(
	char		mtype,
	struct unit_info *uip);
int rbirate(
	struct unit_info *uip);
int fdseek(
	char		mtype,
	struct unit_info *uip,
	int		cylno);
int fdiseek(
	struct unit_info *uip,
	int		cylno);
int mtr_start(
	struct unit_info *uip);
void mtr_on(
	struct unit_info *uip);
void mtr_off(
	struct unit_info *uip);
int sis(
	struct unit_info *uip);
int fdc_sts(
	int		mode,
	struct unit_info *uip);
int fd_getparms(
	int		dev,
	struct disk_parms *diskp);
int fd_setparms(
	unsigned int	unit,
	long		cmdarg);
int fd_format(
	int		dev,
	int		*cmdarg);
int makeidtbl(
	struct fmttbl *tblpt,
	struct fddrtab *dr,
	unsigned short track,
	unsigned short intlv);
void m765sweep(
	struct unit_info *uip,
	struct fddrtab  *cdr);

#define trfrate(uip, type)   outb(VFOREG(uip->addr),(((type)&RATEMASK)>>6))
#define rbskrate(uip, type)  trfrate(uip,(type)&RAPID?RPSEEK:NMSEEK)
#define getparm(type)   ((type<0||type>3)?(struct fddrtab *)ERROR:&m765f[type])
#define relative(s1,s2) ((s1)>(s2)?(s1)-(s2):(s2)-(s1))

boolean_t fdprobe(
	vm_offset_t	port,
	struct bus_device *ctlr)
{
	int			spot = STSREG((int) ctlr->address);
	struct ctrl_info	*cip = &ctrl_info[(unsigned)ctlr->unit];
	int			i, in;

	outb(spot, DATAOK);
	for (i = 1000; i--;) {
	    in = inb(spot);
	    if ((in&DATAOK) == DATAOK && !(in&0x0f)) {
		take_dev_irq(ctlr);
		cip->b_cmd.c_rbmtr = 0;		/* recalibrate/motor flag */
		cip->b_cmd.c_intr = CMDRST;	/* interrupt flag */
		cip->b_unitf = 0;
		cip->b_uip = 0;
		cip->b_rwerr = cip->b_seekerr = cip->b_rberr = 0;
		cip->b_usebuf = FALSE;
		if (FdDmaPage) {
		    cip->b_pbuf = FdDmaPage + PAGE_SIZE * ctlr->unit;
		}
		printf("%s%d: port = %x, spl = %d, pic = %d.\n", ctlr->name,
			ctlr->unit, ctlr->address, ctlr->priority, ctlr->irq);
		return TRUE;
	    }
	}
	return FALSE;
}

boolean_t fdslave(
	struct bus_device *dev,
	vm_offset_t	port)
{
	return TRUE;	/* gross hack */
}

void fdattach(
	struct bus_device *dev)
{
	struct unit_info	*uip = &unit_info[(unsigned)dev->unit];
	struct ctrl_info	*cip = &ctrl_info[(unsigned)dev->ctlr];

	uip->dev = dev;
	uip->ctlr = cip;
	uip->slave = dev->slave;
	dev->address = dev->mi->address;
	uip->addr = dev->address;
	uip->b_cmd = &cip->b_cmd;
	uip->b_seekaddr = 0;
	uip->av_forw = 0;
	uip->wakeme = 0;
	if (cip->b_unitf) {
		uip->b_unitf=cip->b_unitf->b_unitf;
		cip->b_unitf->b_unitf=uip;
	} else {
		uip->b_unitf=uip;
		cip->b_unitf=uip;
	}
	uip->d_drtab.dr_type &= ~OKTYPE; 

	rstout(uip);
	specify(uip);
}
/*****************************************************************************
 *
 * TITLE:	fdopen
 *
 * ABSTRACT:	Open a unit. 
 *
 ****************************************************************************/
io_return_t fdopen(
	int	dev,
	int	flag)			/* not used */
{
	struct fddrtab		*driv;
	struct buf		*wbp;
	spl_t			x = SPL();
	int			error = 0;
	int			unit = UNIT(dev);
	struct unit_info	*uip = &unit_info[unit];
	int			slave = uip->slave;
	struct ctrl_info	*cip = uip->ctlr;
	struct	fdcmd		*cmdp = uip->b_cmd;
	if (unit < MAXUNIT){
	  /* Since all functions that use this are called from open, we only
	     set this once, right here. */
	  	rstout(uip);
		cip->b_wup = uip;
		openchk(cmdp);
		cmdp->c_devflag |= FDMCHK;
		chkbusy(cmdp);
		cmdp->c_stsflag |= MTRFLAG;
		mtr_on(uip);
		if(inb(VFOREG(uip->addr))&OPENBIT || 
		   !(uip->d_drtab.dr_type&OKTYPE)){
			uip->d_drtab.dr_type &= ~OKTYPE;
			if(!rbrate(RAPID, uip))
				fdseek(RAPID, uip, 2);
			if(inb(VFOREG(uip->addr))&OPENBIT)
				error = ENXIO;
		}
		cmdp->c_stsflag &= ~MTRFLAG;
		mtr_on(uip);
		openfre(cmdp);
		if(!error && !(uip->d_drtab.dr_type & OKTYPE)) {
			if (MEDIATYPE(dev)>3)
				goto endopen;
			driv = &m765f[MEDIATYPE(dev)];
			wbp = geteblk(BLKSIZE);
			m765sweep(uip, driv);
			cmdp->c_rbmtr &= ~(1<<(RBSHIFT+(slave)));
			++cip->b_rwerr;
			wbp->b_dev = dev; wbp->b_error = 0; wbp->b_resid = 0;
			wbp->b_flags = (B_READ|B_VERIFY); wbp->b_bcount = 512;
			wbp->b_pfcent = 2*driv->dr_spc + driv->dr_nsec - 1;
			setqueue(wbp, uip);
			biowait(wbp);
			brelse(wbp);
			error = 0;
			uip->d_drtab.dr_type |= OKTYPE;
		}
	} else
		error = ENXIO;
      endopen:
	splx(x);
	return error;
}
/*****************************************************************************
 *
 * TITLE:	fdclose
 *
 * ABSTRACT:	Close a unit.
 *
 *	Called on last close. mark the unit closed and not-ready.
 *
 *****************************************************************************/
io_return_t fdclose(
	int	dev,		/* major, minor numbers */
	int	flag)		/* not used */
{
	struct unit_info	*uip = &unit_info[UNIT(dev)];
	spl_t s;

	/* Clear the bit.
	 * If last close of drive insure drtab queue is empty before returning.
	 */
	s = SPL();
	while(uip->av_forw != 0) {
		uip->wakeme = 1;
		thread_sleep(uip, 0, FALSE);
	}
	splx(s);
	return 0;
}
/*****************************************************************************
 *
 * TITLE:	fdstrategy
 *
 * ABSTRACT:	Queue an I/O Request, and start it if not busy already.
 *
 *	Reject request if unit is not-ready.
 *
 *	Note:	check for not-ready done here ==> could get requests
 *		queued prior to unit going not-ready.
 *		not-ready status to those requests that are attempted
 *		before a new volume is inserted.  Once a new volume is
 *		inserted, would get good I/O's to wrong volume.
 *
 * CALLS:	iodone(),setqueue()
 *
 * CALLING ROUTINES:	fdread (indirectly, thru physio)
 *			fdwrite (indirectly, thru physio)
 *
 ****************************************************************************/
void fdstrategy(
	struct buf *bp)	/* buffer header */
{
	unsigned	        bytes_left;
	daddr_t			secno;
	struct unit_info	*uip = &unit_info[UNIT(bp->b_dev)];
	struct fddrtab		*dr = &uip->d_drtab;
	struct fddrtab		*sdr;

	bp->b_error = 0;
	/* set b_resid to b_bcount because we haven't done anything yet */
	bp->b_resid = bp->b_bcount;
	if (!(dr->dr_type & OKTYPE) || 
	    ((sdr = getparm(MEDIATYPE(bp->b_dev)))==(struct fddrtab *)ERROR) ||
	    /* wrong parameters */
	    (sdr->dr_ncyl != dr->dr_ncyl) || (sdr->dr_nsec != dr->dr_nsec) ||
	    ((sdr->dr_type|OKTYPE) != dr->dr_type) ||
	    (sdr->dr_rwgpl != dr->dr_rwgpl) ||
	    (sdr->dr_fgpl != dr->dr_fgpl)) {
		bp->b_flags |= B_ERROR;
		bp->b_error = EIO;
		biodone(bp);
		return;
	}
	/*
	 * Figure "secno" from b_blkno. Adjust sector # for partition.
	 *
	 * If reading just past the end of the device, it's
	 * End of File.  If not reading, or if read starts further in
	 * than the first sector after the partition, it's an error.
	 *
	 * secno is logical blockno / # of logical blocks per sector */
	secno = (bp->b_blkno * NBPSCTR) >> 9;
	if (secno >= dr->p_nsec) {
		if (!((bp->b_flags & B_READ) && (secno == dr->p_nsec))){
			/* off the deep end */
			bp->b_flags |= B_ERROR;
			bp->b_error = ENXIO;
		}
		biodone(bp);
		return;
	}
/* At this point, it is no longer possible to directly return from strategy.
   We now set b_resid to the number of bytes we cannot transfer because
   they lie beyond the end of the request's partition.  This value is 0
   if the entire request is within the partition. */
	bytes_left = (dr->p_nsec - secno) << 9;
	bp->b_resid = ((bp->b_bcount<=bytes_left)?0:(bp->b_bcount-bytes_left));
	bp->b_pfcent = secno;
	setqueue(bp, uip);
}

/***************************************************************************
 *
 *	set queue to buffer
 *
 ***************************************************************************/
void setqueue(
	struct buf *bp,
	struct unit_info *uip)
{
	spl_t			x = SPL();
	struct ctrl_info	*cip = uip->ctlr;
	struct	fdcmd		*cmdp = uip->b_cmd;

	openchk(cmdp);		/* openning check */
	cmdp->c_devflag |= STRCHK;
	fd_disksort(uip, bp);	/* queue the request */
	/*
	 * If no requests are in progress, start this one up.  Else
	 * leave it on the queue, and fdintr will call m765io later.
	 */
	if(!cip->b_uip)
		m765io(uip);
	splx(x);
}
/***************************************************************************
 *
 *	check io_busy routine
 *
 ***************************************************************************/
void chkbusy(
	struct fdcmd	*cmdp)
{
	while(cmdp->c_devflag & STRCHK){
		cmdp->c_devflag |= STRWAIT;
		thread_sleep(&cmdp->c_devflag, 0, FALSE);
	} 
}
/***************************************************************************
 *
 *	check fdopen() routine
 *
 ***************************************************************************/
void openchk(
	struct fdcmd	*cmdp)
{
	while(cmdp->c_devflag & FDMCHK ){
		cmdp->c_devflag |= FDWAIT;
		thread_sleep(&cmdp->c_devflag, 0, FALSE);
	} 
}
/***************************************************************************
 *
 *	free fdopen() routine
 *
 ***************************************************************************/
void openfre(
	struct fdcmd	*cmdp)
{
	cmdp->c_devflag &= ~FDMCHK;
	if(cmdp->c_devflag & FDWAIT){
		cmdp->c_devflag &= ~FDWAIT;
		thread_wakeup(&cmdp->c_devflag);
	}
}
/*****************************************************************************
 *
 * TITLE:	m765io
 *
 * ABSTRACT:	Start handling an I/O request.
 *
 ****************************************************************************/
void m765io(
	struct unit_info *uip)
{
	register struct buf *bp;
	struct ctrl_info *cip = uip->ctlr;

	bp = uip->av_forw; /*move bp to ctrl_info[ctrl].b_buf*/
	cip->b_buf = bp;
	cip->b_uip = uip;
	cip->b_xferaddr  = (vm_offset_t) bp->b_un.b_addr;
	cip->b_xfercount = bp->b_bcount - bp->b_resid;
	cip->b_sector    = bp->b_pfcent;
	uip->b_cmd->c_stsflag |= MTRFLAG;
	if(!mtr_start(uip))
		timeout(m765iosub, uip, hz);
	else
		m765iosub(uip);
}
/****************************************************************************
 *
 *	m765io subroutine
 *
 ****************************************************************************/
void m765iosub(
	struct unit_info *uip)
{
	struct fddrtab		*dr = &uip->d_drtab;
	int			startsec;
	int			slave = uip->slave;
	struct ctrl_info	*cip = uip->ctlr;
	struct	fdcmd		*cmdp = uip->b_cmd;

	rwcmdset(uip);
	if(cip->b_buf->b_flags&B_FORMAT)
		goto skipchk;
	startsec = (cmdp->c_rwdata[3] * dr->dr_nsec) + cmdp->c_rwdata[4];
	if(startsec+(cip->b_xfercount>>9)-1 > dr->dr_spc)
		cip->b_xferdma = (dr->dr_spc-startsec+1) << 9;
	else
skipchk:	cip->b_xferdma = cip->b_xfercount;
	if(!(cmdp->c_rbmtr & (1<<(RBSHIFT+slave))))
       		cip->b_status = rbirate(uip);
	else if(uip->b_seekaddr != cmdp->c_saddr)
		cip->b_status = fdiseek(uip,cmdp->c_saddr);
	else
		cip->b_status = outicmd(uip);
	if(cip->b_status)
		intrerr0(uip);
}
/***************************************************************************
 *
 *	read / write / format / verify command set to command table
 *
 ***************************************************************************/
void rwcmdset(
	struct unit_info *uip)
{
	short			resid;
	int			slave = uip->slave;
	struct ctrl_info	*cip = uip->ctlr;
	struct fdcmd		*cmdp = uip->b_cmd;

	switch(cip->b_buf->b_flags&(B_FORMAT|B_VERIFY|B_READ|B_WRITE)){
	case B_VERIFY|B_WRITE:	/* VERIFY after WRITE */
		cmdp->c_rwdata[0] = RDMV;
		break;
	case B_FORMAT:
		cmdp->c_dcount = FMTCNT; 
		cmdp->c_rwdata[0] = FMTM;
		cmdp->c_saddr = cip->b_sector / uip->d_drtab.dr_spc;
		resid = cip->b_sector % uip->d_drtab.dr_spc;
		cmdp->c_rwdata[1] = slave|((resid/uip->d_drtab.dr_nsec)<<2);
		cmdp->c_rwdata[2] = 
			((struct fmttbl *)cip->b_buf->b_un.b_addr)->s_type;
		cmdp->c_rwdata[3] = uip->d_drtab.dr_nsec;
		cmdp->c_rwdata[4] = uip->d_drtab.dr_fgpl;
		cmdp->c_rwdata[5] = FMTDATA;
		break;
	case B_WRITE:
	case B_READ:
	case B_READ|B_VERIFY:
		cmdp->c_dcount = RWCNT;
		if(cip->b_buf->b_flags&B_READ)
			if(cip->b_buf->b_flags&B_VERIFY)
				cmdp->c_rwdata[0] = RDMV;
			else
				cmdp->c_rwdata[0] = RDM;
		else
			cmdp->c_rwdata[0] = WTM;	/* format or write */
		resid = cip->b_sector % uip->d_drtab.dr_spc;
		cmdp->c_rwdata[3] = resid / uip->d_drtab.dr_nsec;
		cmdp->c_rwdata[1] = slave|(cmdp->c_rwdata[3]<<2);
		cmdp->c_rwdata[2] = cmdp->c_saddr = 
			cip->b_sector / uip->d_drtab.dr_spc;
		cmdp->c_rwdata[4] = (resid % uip->d_drtab.dr_nsec) + 1;
		cmdp->c_rwdata[5] = 2;
		cmdp->c_rwdata[6] = uip->d_drtab.dr_nsec;
		cmdp->c_rwdata[7] = uip->d_drtab.dr_rwgpl;
		cmdp->c_rwdata[8] = DTL;
		D(printf("SET %x %x C%x H%x S%x %x %x %x %x ",
			cmdp->c_rwdata[0], cmdp->c_rwdata[1],
			cmdp->c_rwdata[2], cmdp->c_rwdata[3],
			cmdp->c_rwdata[4], cmdp->c_rwdata[5],
			cmdp->c_rwdata[6], cmdp->c_rwdata[7],
			cmdp->c_rwdata[8]));
		break;
	}
}
/*****************************************************************************
 *
 * TITLE:	fdread
 *
 * ABSTRACT:	"Raw" read.  Use physio().
 *
 * CALLS:	m765breakup (indirectly, thru physio)
 *
 ****************************************************************************/
io_return_t fdread(
	int	dev,
	struct buf *buf)
{ 
	/* no need for page-size restriction */
	return block_io(fdstrategy, minphys, buf);
}
/*****************************************************************************
 *
 * TITLE:	fdwrite
 *
 * ABSTRACT:	"Raw" write.  Use physio().
 *
 * CALLS:	m765breakup (indirectly, thru physio)
 *
 ****************************************************************************/
io_return_t fdwrite(
	int	dev,
	struct buf *buf)
{
	/* no need for page-size restriction */
	return block_io(fdstrategy, minphys, buf);
}

/* IOC_OUT only and not IOC_INOUT */
io_return_t fdgetstat(
	int		dev,
	dev_flavor_t	flavor,
	dev_status_t	data,		/* pointer to OUT array */
	natural_t	*count)		/* OUT */
{
	switch (flavor) {

	/* Mandatory flavors */

	case DEV_GET_SIZE: {
		int ret;
		struct disk_parms	p;

		ret = fd_getparms(dev, &p);
		if (ret) return ret;
		data[DEV_GET_SIZE_DEVICE_SIZE] = p.dp_pnumsec * NBPSCTR;
		data[DEV_GET_SIZE_RECORD_SIZE] = NBPSCTR;
		*count = DEV_GET_SIZE_COUNT;
		break;
	}

	/* Extra flavors */

	case V_GETPARMS:
		if (*count < sizeof (struct disk_parms)/sizeof (int))
			return D_INVALID_OPERATION;
		*count = sizeof (struct disk_parms)/sizeof(int);
		return fd_getparms(dev,
				   (struct disk_parms *)data);
        default:
		return D_INVALID_OPERATION;
	}
	return D_SUCCESS;
}

/* IOC_VOID or IOC_IN or IOC_INOUT */
/*ARGSUSED*/
io_return_t fdsetstat(
	int		dev,
	dev_flavor_t	flavor,
	dev_status_t	data,
	natural_t	count)
{
	int			unit = UNIT(dev);
	switch (flavor) {
	case V_SETPARMS:    /* Caller wants reset_parameters */
		return fd_setparms(unit,*(int *)data);
	case V_FORMAT:
		return fd_format(dev,data);
	case V_VERIFY:	/* cmdarg : 0 == no verify, 0 != verify */
		m765verify[unit] = *(int *)data;
		return D_SUCCESS;
	default:
		return D_INVALID_OPERATION;
	}
}

/*
 *      Get block size
 */
int fddevinfo(
	dev_t   dev,
	int	flavor,
	char	*info)
{
	register struct	fddrtab	*dr;
	register int result = D_SUCCESS;

	switch (flavor) {
	case D_INFO_BLOCK_SIZE:
		dr = &unit_info[UNIT(dev)].d_drtab;

	        if(dr->dr_type & OKTYPE)
        	        *((int *) info) =  512;
		else
			result = D_INVALID_OPERATION;

		break;
	default:
		result = D_INVALID_OPERATION;
	}

        return result;
}
/****************************************************************************
 *
 *	set fd parameters 
 *
 ****************************************************************************/
int
fd_setparms(
	register unsigned int unit,
	long cmdarg)
{
	struct fddrtab	 *fdparm;
	spl_t		 x;
	struct unit_info *uip = &unit_info[unit];
	struct	fdcmd	 *cmdp = uip->b_cmd;

	cmdp->c_rbmtr &= ~(1<<(RBSHIFT+uip->slave));
	if ((fdparm = getparm(MEDIATYPE(cmdarg))) == (struct fddrtab *)ERROR)
	        return EINVAL;
	x = SPL();
	openchk(cmdp);
	cmdp->c_devflag |= FDMCHK;
	chkbusy(cmdp);
	m765sweep(uip, fdparm);
	uip->d_drtab.dr_type |= OKTYPE;
	openfre(cmdp);
	splx(x);
	return 0;
}
/****************************************************************************
 *
 *	get fd parameters 
 *
 ****************************************************************************/
int fd_getparms(
	int		dev,
	struct disk_parms *diskp)
{
	register struct	fddrtab	*dr = &unit_info[UNIT(dev)].d_drtab;

	if(dr->dr_type & OKTYPE){
		diskp->dp_type = DPT_FLOPPY;
		diskp->dp_heads = 2;
		diskp->dp_sectors = dr->dr_nsec;
		diskp->dp_pstartsec = 0;
		diskp->dp_cyls = dr->dr_ncyl;
		diskp->dp_pnumsec = dr->p_nsec;
		return 0;
	}
	return ENXIO;
}
/****************************************************************************
 *
 *	format command
 *
 ****************************************************************************/
int fd_format(
	int	dev,		/* major, minor numbers */
	int	*cmdarg)
{
	register struct buf	*bp;
	register daddr_t	track;
	union  io_arg		*varg;
	u_short			num_trks;
	register struct	fddrtab	*dr = &unit_info[UNIT(dev)].d_drtab;

	if(!(dr->dr_type & OKTYPE))
		return EINVAL;	
	varg = (union io_arg *)cmdarg;
	num_trks = varg->ia_fmt.num_trks;
	track = (daddr_t)(varg->ia_fmt.start_trk*dr->dr_nsec);
	if((track + (num_trks*dr->dr_nsec))>dr->p_nsec)
		return EINVAL;
	bp = geteblk(BLKSIZE);		/* get struct buf area */
	while (num_trks>0) {
		bp->b_flags &= ~B_DONE;
		bp->b_dev = dev;
		bp->b_error = 0; bp->b_resid = 0;
		bp->b_flags = B_FORMAT;	
		bp->b_bcount = dr->dr_nsec * FMTID;
		bp->b_blkno = (daddr_t)((track << 9) / NBPSCTR);
		if(makeidtbl((struct fmttbl *)bp->b_un.b_addr,dr,
			     varg->ia_fmt.start_trk++,varg->ia_fmt.intlv))
			return EINVAL;
		fdstrategy(bp);
		biowait(bp);
		if(bp->b_error)
			if((bp->b_error == (char)EBBHARD) || 
			   (bp->b_error == (char)EBBSOFT))
				return EIO;
			else
				return bp->b_error;
		num_trks--;
		track += dr->dr_nsec;
	}
	brelse(bp);
	return 0;
}
/****************************************************************************
 *
 *	make id table for format
 *
 ****************************************************************************/
int makeidtbl(
	struct fmttbl *tblpt,
	struct fddrtab *dr,
	unsigned short track,
	unsigned short intlv)
{
	register int	i,j,secno;

	if(intlv >= dr->dr_nsec)
		return 1;
	for(i=0; i<dr->dr_nsec; i++)
		tblpt[i].sector = 0;
	for(i=0,j=0,secno=1; i<dr->dr_nsec; i++){
		tblpt[j].cyl = track >> 1;
		tblpt[j].head = track & 1;
		tblpt[j].sector = secno++;
		tblpt[j].s_type = 2;
		if((j+=intlv) < dr->dr_nsec)
			continue;
		for(j-=dr->dr_nsec; j < dr->dr_nsec ; j++)
			if(!tblpt[j].sector)
				break;
	}
	return 0;
}
/*****************************************************************************
 *
 * TITLE:	fdintr
 *
 * ABSTRACT:	Handle interrupt.
 *
 *	Interrupt procedure for m765 driver.  Gets status of last
 *	operation and performs service function according to the
 *	type of interrupt.  If it was an operation complete interrupt,
 *	switches on the current driver state and either declares the
 *	operation done, or starts the next operation
 *
 ****************************************************************************/
void fdintr(
	int	ctrl)
{
	struct unit_info 	*uip = ctrl_info[ctrl].b_uip;
	struct unit_info 	*wup = ctrl_info[ctrl].b_wup;
	struct fdcmd		*cmdp = &ctrl_info[ctrl].b_cmd;
	if(cmdp->c_stsflag & INTROUT)
		untimeout(fdintr, (void *) ctrl);
	cmdp->c_stsflag &= ~INTROUT;	
	switch(cmdp->c_intr){
	case RWFLAG:
		rwintr(uip);
		break;	
	case SKFLAG:
	case SKEFLAG|SKFLAG:
	case RBFLAG:
		timeout(m765intrsub, uip, SEEKWAIT);
		break;
	case WUPFLAG:
		cmdp->c_intr &= ~WUPFLAG;
		thread_wakeup(wup);
	}
}
/*****************************************************************************
 *
 *	interrupt subroutine (seek recalibrate)
 *
 *****************************************************************************/
void m765intrsub(
	struct unit_info *uip)
{
	struct ctrl_info *cip = uip->ctlr;

	if((cip->b_status = sis(uip))!=  ST0OK)
		switch(uip->b_cmd->c_intr){
		case SKFLAG:
			seekintr(uip);
			break;
		case SKEFLAG|SKFLAG:
			seekintre(uip);
			break;
		case RBFLAG:
			rbintr(uip);
		}
}
/*****************************************************************************
 *
 *	read / write / format / verify interrupt routine
 *
 *****************************************************************************/
void rwintr(
	struct unit_info *uip)
{
	int			rsult[7];
	register int		rtn, count;
	struct ctrl_info	*cip = uip->ctlr;
	struct fdcmd		*cmdp = uip->b_cmd;

	cmdp->c_intr &= ~RWFLAG;
	if((cip->b_buf->b_flags&(B_READ|B_VERIFY))!=(B_READ|B_VERIFY))
		if(inb(VFOREG(uip->addr))&OPENBIT){
			if(cip->b_buf->b_flags&B_FORMAT){
				cip->b_status = TIMEOUT;
				intrerr0(uip);
			} else {
				if((inb(STSREG(uip->addr))&ST0OK)!=ST0OK)
					printf("%s %d : %s\n",
						fderr,
						uip-unit_info,
						fdmsg[DOORERR]);
				rstout(uip);
				specify(uip);
				cmdp->c_rbmtr &= RBRST;
				cmdp->c_intr |= SKEFLAG;
				if(cmdp->c_saddr > 2)
					fdiseek(uip, cmdp->c_saddr-2);
				else
					fdiseek(uip, cmdp->c_saddr+2);
			}
			return;
		}
	for( count = 0 ; count < 7 ; count++ ){
		if ((rtn = fdc_sts(FD_ISTS, uip)) != 0)	/* status check */
			goto rwend;
		rsult[count] = inb(DATAREG(uip->addr));
	}
	rtn = 0;
	if(rsult[0]&0xc0){
		rtn = cmdp->c_rwdata[0]<<8;
		if(rsult[0]&0x80){ rtn |= FDCERR;   goto rwend; }
		if(rsult[1]&0x80){ rtn |= NOREC;    goto rwend; }
		if(rsult[1]&0x20){ rtn |= CRCERR;   goto rwend; }
		if(rsult[1]&0x10){ rtn |= OVERRUN;  goto rwend; }
		if(rsult[1]&0x04){ rtn |= NOREC;    goto rwend; }
		if(rsult[1]&0x02){ rtn |= WTPRT;    goto rwend; }
		if(rsult[1]&0x01){ rtn |= ADDRERR;  goto rwend; }
		rtn |= FDCERR;
rwend:		outb(0x0a, 0x06);
	}
	if ((cip->b_status = rtn) != 0) {
		D(printf("\n->rwierr %x ", rtn));
		rwierr(uip);
	} else { /* write command */
		if(((cip->b_buf->b_flags&(B_FORMAT|B_READ|B_WRITE))==B_WRITE) 
		   && !(cip->b_buf->b_flags & B_VERIFY)) {
			D(printf("->w/v "));
			cip->b_buf->b_flags |= B_VERIFY;
			rwcmdset(uip);
			if ((cip->b_status = outicmd(uip)) != 0)
				intrerr0(uip);
			return;
		}
		/* clear retry count */
		if (cip->b_usebuf) {
			bcopy((char *)phystokv(cip->b_pbuf),
			      (char *)cip->b_xferaddr,
			      cip->b_xferdma);
			DD(printf("R(%x, %x, %x)\n",
				cip->b_pbuf, cip->b_xferaddr, cip->b_xferdma));
		}
		cip->b_buf->b_flags &= ~B_VERIFY;
		cip->b_rwerr = cip->b_seekerr = cip->b_rberr = 0;
		cip->b_xfercount -= cip->b_xferdma;
		cip->b_xferaddr += cip->b_xferdma;
		cip->b_sector = cip->b_sector+(cip->b_xferdma>>9);
		D(printf("->done%s\n", cip->b_xfercount?"":"." ));
		/* next address (cyl,head,sec) */
		if((int)cip->b_xfercount>0)
			m765iosub(uip);
		else
			quechk(uip);
	}
}
/*****************************************************************************
 *
 *	read / write / format / verify error routine
 *
 *****************************************************************************/
void rwierr(
	struct unit_info *uip)
{
	struct ctrl_info	*cip = uip->ctlr;
	struct	fdcmd		*cmdp = uip->b_cmd;
	short			status = cip->b_status;

	D(printf("%x-%x-%x ", cip->b_rwerr&SRMASK, cip->b_rwerr&MRMASK, cip->b_rwerr&LRMASK));
	if((cip->b_buf->b_flags&(B_READ|B_VERIFY))==(B_READ|B_VERIFY)){
		if((cip->b_rwerr&SRMASK)<MEDIARD)
			goto rwrtry;
		if((cip->b_rwerr&MRMASK)<MEDIASEEK)
			goto rwseek;
		goto rwexit;
	} else
		if(cip->b_buf->b_flags&B_VERIFY){
			cip->b_buf->b_flags &= ~B_VERIFY;
			rwcmdset(uip);
		}
rwrtry:
	if((++cip->b_rwerr&SRMASK)<SRETRY)
		cip->b_status = outicmd(uip);
	else {
rwseek:		cip->b_rwerr = (cip->b_rwerr&RMRMASK)+MINC;
		if((cip->b_rwerr&MRMASK)<MRETRY){
			cmdp->c_intr |= SKEFLAG;
			if(cmdp->c_saddr > 2)
				cip->b_status=fdiseek(uip,cmdp->c_saddr-2);
			else
				cip->b_status=fdiseek(uip,cmdp->c_saddr+2);
		} else {
			cip->b_rwerr = (cip->b_rwerr&LRMASK)+LINC;
			if((cip->b_rwerr&LRMASK)<LRETRY)
       				cip->b_status=rbirate(uip);
		}
	}
	if(cip->b_status){
		D(printf("ERR->intrerr0 "));
		cip->b_status = status;
rwexit:		intrerr0(uip);
	}
}
/*****************************************************************************
 *
 *	recalibrate interrupt routine
 *
 *****************************************************************************/
void rbintr(
	struct unit_info *uip)
{
	struct ctrl_info	*cip = uip->ctlr;
	struct fdcmd		*cmdp = uip->b_cmd;

	cmdp->c_intr &= ~RBFLAG;
	if(cip->b_status) {
		if(++cip->b_rberr<SRETRY)
			cip->b_status = rbirate(uip);
	} else {
		cmdp->c_rbmtr |= 1<<(RBSHIFT+uip->slave);
		uip->b_seekaddr = 0;
		cip->b_rberr = 0;
		cip->b_status=fdiseek(uip, cmdp->c_saddr);
	}
	if(cip->b_status)
		intrerr0(uip);
}
/******************************************************************************
 *
 *	seek interrupt routine
 *
 *****************************************************************************/
void seekintr(
	struct unit_info *uip)
{
	struct ctrl_info	*cip = uip->ctlr;
	struct fdcmd		*cmdp = uip->b_cmd;

	cmdp->c_intr &= ~SKFLAG;
	if(cip->b_status)
		seekierr(uip, cmdp->c_saddr);
	else {
		uip->b_seekaddr = cmdp->c_saddr;
		cip->b_status = outicmd(uip);
	}
	if(cip->b_status)
		intrerr0(uip);
	else
		cip->b_seekerr = 0;
}
/*****************************************************************************
 *
 *	seek error retry interrupt routine
 *
 *****************************************************************************/
void seekintre(
	struct unit_info *uip)
{
	register char		seekpoint;
	struct ctrl_info	*cip = uip->ctlr;
	struct fdcmd		*cmdp = uip->b_cmd;

	cmdp->c_intr &= ~(SKEFLAG|SKFLAG);
	if(cmdp->c_saddr > 2)
		seekpoint = cmdp->c_saddr-2;
	else
		seekpoint = cmdp->c_saddr+2;
	if(cip->b_status)
		seekierr(uip, seekpoint);
	else {
		uip->b_seekaddr = seekpoint;
		cip->b_status = fdiseek(uip, cmdp->c_saddr);
	}
	if(cip->b_status)
		intrerr0(uip);
	else
		cip->b_seekerr = 0;
}
/*****************************************************************************
 *
 *	seek error routine
 *
 *****************************************************************************/
void seekierr(
	struct unit_info *uip,
	register char 	seekpoint)
{
	struct ctrl_info *cip = uip->ctlr;

	if((++cip->b_seekerr&SRMASK)<SRETRY)
		cip->b_status=fdiseek(uip, seekpoint);
	else {
		cip->b_seekerr = (cip->b_seekerr&MRMASK) + MINC;
		if((cip->b_seekerr&MRMASK)<MRETRY)
			cip->b_status=rbirate(uip);
	}
	if(cip->b_status)
		intrerr0(uip);
}
/*****************************************************************************
 *
 * TITLE:	m765sweep
 *
 * ABSTRACT:	Perform an initialization sweep.  
 *
 **************************************************************************/
void m765sweep(
	struct unit_info *uip,
	register struct fddrtab  *cdr)	/* device initialization data */
{
	register struct fddrtab *dr = &uip->d_drtab;

	dr->dr_ncyl = cdr->dr_ncyl;
	dr->dr_nsec = cdr->dr_nsec;
	dr->dr_spc  = cdr->dr_spc;
	dr->p_nsec  = cdr->p_nsec;
	dr->dr_type = cdr->dr_type;
	dr->dr_rwgpl= cdr->dr_rwgpl;
	dr->dr_fgpl = cdr->dr_fgpl;
}
/*****************************************************************************
 *
 *  TITLE:  m765disksort
 *
 *****************************************************************************/
void fd_disksort(
	struct unit_info *uip,	 /*  Pointer to head of active queue	*/
	register struct buf *bp) /*  Pointer to buffer to be inserted	*/
{
	register struct buf *bp2; /*  Pointer to next buffer in queue	*/
	register struct buf *bp1; /*  Pointer where to insert buffer	*/

	if (!(bp1 = uip->av_forw)) {
		/* No other buffers to compare against */
		uip->av_forw = bp;
		bp->av_forw = 0;
		return;
	}
	bp2 = bp1->av_forw;
	while(bp2 && (relative(bp1->b_pfcent,bp->b_pfcent) >=
		      relative(bp1->b_pfcent,bp2->b_pfcent))) {
		bp1 = bp2;
		bp2 = bp1->av_forw;
	}
	bp1->av_forw = bp;
	bp->av_forw = bp2;
}
/*****************************************************************************
 *
 *	Set Interrupt error and FDC reset
 *
 *****************************************************************************/
void intrerr0(
	struct unit_info *uip)
{
	struct buf		*bp; /* Pointer to next buffer in queue	*/
	int			resid;
	struct ctrl_info	*cip = uip->ctlr;
	struct	fdcmd		*cmdp = uip->b_cmd;
	register struct	fddrtab *dr = &uip->d_drtab;

	if((cip->b_buf->b_flags&(B_READ|B_VERIFY))!=(B_READ|B_VERIFY)){
		/*
		 *	Read count not transfered: low, then high
		 */
		resid = inb(DMACNT);
		resid = resid | (inb(DMACNT) << 8);
		resid = cip->b_xfercount = cip->b_xferdma-1-resid;
		resid = (cip->b_sector + (resid>>9)) % dr->dr_spc;
		printf("%s %d : %s\n",
			fderr,
			uip->slave,
			fdmsg[cip->b_status&BYTEMASK]);
		printf("cylinder = %d  ",cmdp->c_saddr);
		printf("head = %d  sector = %d  byte/sec = %d\n",
		resid / dr->dr_nsec , (resid % dr->dr_nsec)+1 , 512);
	}
	cip->b_rwerr = cip->b_seekerr = cip->b_rberr = 0;
	cmdp->c_intr = CMDRST;
	if(((cip->b_buf->b_flags&(B_READ|B_VERIFY))!=(B_READ|B_VERIFY)) &&
	   uip->slave)
		dr->dr_type &= ~OKTYPE; 
	bp = cip->b_buf;
	bp->b_flags |= B_ERROR;
	switch(cip->b_status&BYTEMASK){
	case ADDRERR:
	case OVERRUN:
	case FDCERR:
	case TIMEOUT:
		bp->b_error = EIO;
		break;
	case WTPRT:
		bp->b_error = ENXIO;
		break;
	case NOREC:
		bp->b_error = EBBHARD;
		break;
	case CRCERR:
		bp->b_error = EBBSOFT;
	}
	rstout(uip);
	specify(uip);
	cmdp->c_rbmtr &= RBRST;
	quechk(uip);
}
/*****************************************************************************
 *
 *	Next queue check routine
 *
 *****************************************************************************/
void quechk(
	struct unit_info *uip)
{
	register struct	buf 	*bp = uip->av_forw;
	struct ctrl_info	*cip = uip->ctlr;
	struct unit_info        *loop;
	struct fdcmd		*cmdp = uip->b_cmd;
	/* clear retry count */
	cip->b_rwerr = cip->b_seekerr = cip->b_rberr = 0;
	bp->b_resid = bp->b_resid + cip->b_xfercount;
	uip->av_forw=bp->av_forw;
	if (!uip->av_forw && uip->wakeme) {
		uip->wakeme = 0;
		thread_wakeup(uip);
	}
	biodone(bp);
	loop = uip;
	do {
		loop=loop->b_unitf;
		if (loop->av_forw) {
			m765io(loop);
			return;
		}
	} while (loop!=uip);
	cip->b_uip = 0;
	cmdp->c_stsflag &= ~MTRFLAG;
	mtr_on(uip);
	cmdp->c_devflag &= ~STRCHK;
	if(cmdp->c_devflag & STRWAIT){
		cmdp->c_devflag &= ~STRWAIT;
		thread_wakeup(&cmdp->c_devflag);
	}
}
void fdprint(
	int	dev,
	char	*str)
{
	printf("floppy disk driver: %s on bad dev %d, partition %d\n",
			str, UNIT(dev), 0);
}

/*****************************************************************************
 *
 *	fdc reset routine
 *
 *****************************************************************************/
void rstout(
	struct unit_info *uip)
{
	register int	outd;

	outd = ((uip->b_cmd->c_rbmtr&MTRMASK)<<MTR_ON)|uip->slave;
	outb(CTRLREG(uip->addr), outd);
	outd |= FDC_RST;
	outb(CTRLREG(uip->addr), outd);
	outd |= DMAREQ;
	outb(CTRLREG(uip->addr), outd);
}
/*****************************************************************************
 *
 *	specify command routine
 *
 *****************************************************************************/
void specify(
	struct unit_info *uip)
{
	/* status check */
	if(fdc_sts(FD_OSTS, uip))
		return;
	/* Specify command */
	outb(DATAREG(uip->addr), SPCCMD);
	/* status check */
	if(fdc_sts(FD_OSTS, uip))
		return;
	/* Step rate,Head unload time */
	outb(DATAREG(uip->addr), SRTHUT);
	/* status check */
	if(fdc_sts(FD_OSTS, uip))
		return;
	/* Head load time,Non DMA Mode*/
	outb(DATAREG(uip->addr), HLTND);
	return;
}
/****************************************************************************
 *
 *	recalibrate command routine
 *
 ****************************************************************************/
int rbrate(
	char	 mtype,
	struct unit_info *uip)
{
	register int	rtn = 1, rty_flg=2;
	spl_t		x;
	struct	fdcmd	*cmdp = uip->b_cmd;

	rbskrate(uip, mtype);			/* set transfer rate */
	while((rty_flg--)&&rtn){
		if ((rtn = fdc_sts(FD_OSTS, uip)) != 0)	/* status check */
			break;
		/*recalibrate command*/
		outb(DATAREG(uip->addr), RBCMD);
		if ((rtn = fdc_sts(FD_OSTS, uip)) != 0)	/* status check */
			break;
		/* Device to wake up specified in open */
		cmdp->c_intr |= WUPFLAG;
		x = SPL();
		outb(DATAREG(uip->addr), uip->slave);
		rtn = ERROR;
		while(rtn) {
			uip->wakeme = 1;
			thread_sleep(uip, 0, FALSE);
			if((rtn = sis(uip)) == ST0OK)
			  /* Device to wake up specified in open */
				cmdp->c_intr |= WUPFLAG;
			else
				break;
		}
		splx(x);
	}
	return rtn;
}
/*****************************************************************************
 *
 *	seek command routine
 *
 ****************************************************************************/
int fdseek(
	register char	mtype,
	struct unit_info *uip,
	register int	cylno)
{
	spl_t		x;
	int		rtn;
	struct	fdcmd	*cmdp = uip->b_cmd;

	rbskrate(uip, mtype);
	if ((rtn = fdc_sts(FD_OSTS, uip)) != 0)		/* status check */
		return rtn;
	outb(DATAREG(uip->addr), SEEKCMD);		/* seek command */
	if ((rtn = fdc_sts(FD_OSTS, uip)) != 0)		/* status check */
		return rtn;
	outb(DATAREG(uip->addr), uip->slave);		/* drive number */
	if ((rtn = fdc_sts(FD_OSTS, uip)) != 0)		/* status check */
		return rtn;
	x = SPL();
	/* Device to wake up specified in open */
	cmdp->c_intr |= WUPFLAG;
	outb(DATAREG(uip->addr), cylno);		/* seek count */
	rtn = ERROR;
	while(rtn){	
		uip->wakeme = 1;
		thread_sleep(uip, 0, FALSE);
		if ((rtn = sis(uip)) == ST0OK)
		  /* Device to wake up specified in open */
			cmdp->c_intr |= WUPFLAG;
		else
			break;
	}
	splx(x);
	return rtn;
}
/*****************************************************************************
 *
 *	seek commnd routine(use interrupt)
 *
 *****************************************************************************/
int fdiseek(
	struct unit_info *uip,
	int	cylno)
{
	register int	rtn;

	D(printf("SK %x ", cylno));
	rbskrate(uip, uip->d_drtab.dr_type);		/* set transfer rate */
	if ((rtn = fdc_sts(FD_OSTS, uip)) != 0)		/* status check */
		goto fdiend;
	outb(DATAREG(uip->addr), SEEKCMD);		/* seek command */
	if ((rtn = fdc_sts(FD_OSTS, uip)) != 0)		/* status check */
		goto fdiend;
	outb(DATAREG(uip->addr), uip->slave);		/* drive number */
	if ((rtn = fdc_sts(FD_OSTS, uip)) != 0)		/* status check */
		goto fdiend;
	uip->b_seekaddr = cylno;
	if(uip->d_drtab.dr_type&DOUBLE)
		cylno = cylno * 2;
	uip->b_cmd->c_intr |= SKFLAG;
	outb(DATAREG(uip->addr), cylno);	/* seek count */
fdiend:	
	if(rtn)
		rtn |= SEEKCMD<<8;
	return rtn;
}
/*****************************************************************************
 *
 *	recalibrate command routine(use interrupt)
 *
 *****************************************************************************/
int rbirate(
	struct unit_info *uip)
{
	register int	rtn;

	rbskrate(uip, uip->d_drtab.dr_type);/* set transfer rate */
	if(!(rtn = fdc_sts(FD_OSTS, uip))) {		/* status check */
		/* recalibrate command */
		outb(DATAREG(uip->addr), RBCMD);
		if(!(rtn = fdc_sts(FD_OSTS, uip))) {	/* status check */
			uip->b_cmd->c_intr |= RBFLAG;
			outb(DATAREG(uip->addr), uip->slave);
		}
	}
	return (rtn) ? rtn|RBCMD<<8 : 0;
}
/*****************************************************************************
 *
 *	read / write / format / verify command out routine(use interrupt)
 *
 *****************************************************************************/
int outicmd(
	struct unit_info *uip)
{
	int			rtn;
	register int		*data,cnt0,dmalen;
	vm_offset_t		address;
	struct ctrl_info	*cip = uip->ctlr;
	struct fdcmd		*cmdp = uip->b_cmd;
	spl_t			x = splhigh();

	outb(DMACMD1,DMADATA0);	/* DMA #1 command register 	*/
	outb(DMAMSK1,DMADATA1);	/* DMA #1 all mask register	*/
	/* Perhaps outb(0x0a,0x02); might work better on line above? */
	switch(cmdp->c_rwdata[0]){
	case RDM:
		D(printf("RDM"));
		outb(DMABPFF,DMARD);
		outb(DMAMODE,DMARD);
		break;
	case WTM:
	case FMTM:
		D(printf("W"));
		outb(DMABPFF,DMAWT);
		outb(DMAMODE,DMAWT);
		break;
	case RDMV:
		D(printf("RDMV"));
		outb(DMABPFF,DMAVRF);
		outb(DMAMODE,DMAVRF);
	}
	/* get work buffer physical address */
	address = kvtophys(cip->b_xferaddr);
	dmalen = i386_trunc_page(address) + I386_PGBYTES - address;
	if ( (cip->b_rwerr&MRMASK) >= 0x10)
		dmalen = 0x200;
	if (dmalen<=cip->b_xferdma) 
		cip->b_xferdma = dmalen;
	else
		dmalen = cip->b_xferdma;
	if (address >= FdDmaThreshold) {
		DD(printf(">(%x[%x], %x[%x] L%x\n",
			address, cip->b_pbuf,
			cip->b_xferaddr, cip->b_pbuf, dmalen));
		if (!FdDmaEISA) {
			cip->b_usebuf = TRUE;
			address = cip->b_pbuf;
			if (cmdp->c_rwdata[0] == WTM ||
			    cmdp->c_rwdata[0] == FMTM) {
				bcopy((void *)cip->b_xferaddr,
				      (void *)phystokv(cip->b_pbuf),
				      dmalen);
				DD(printf("W(%x, %x, %x)\n",
					cip->b_xferaddr, cip->b_pbuf, dmalen));
			}
		} else
			cip->b_usebuf = FALSE;
	} else
		cip->b_usebuf = FALSE;
	D(printf(" %x L%x ", address, dmalen));
	/* set buffer address */
	outb(DMAADDR,address&BYTEMASK);
	outb(DMAADDR,(address>>8)&BYTEMASK);
	outb(DMAPAGE,(address>>16)&BYTEMASK);
	if (FdDmaEISA)
		outb(FdDmaEISA+DMAPAGE-0x80,(address>>24)&BYTEMASK);
	/* set transfer count */
	outb(DMACNT,(--dmalen)&BYTEMASK);
	outb(DMACNT,(dmalen>>8)&BYTEMASK);
	outb(DMAMSK,CHANNEL2);
	splx(x);
	trfrate(uip, uip->d_drtab.dr_type);	/* set transfer rate */
	data = &cmdp->c_rwdata[0];
	for(cnt0 = 0; cnt0<cmdp->c_dcount; cnt0++,data++){
		if ((rtn = fdc_sts(FD_OSTS, uip)) != 0)	/*status check*/
			return rtn;
		outb(DATAREG(uip->addr), *data);
	}
	cmdp->c_intr |= RWFLAG;
	cmdp->c_stsflag |= INTROUT;
	cnt0 = ((cip->b_buf->b_flags&(B_READ|B_VERIFY)) ==
		(B_READ|B_VERIFY))?TOUT:ITOUT;
	timeout(fdintr,(void *)(int)uip->dev->ctlr,cnt0);
	return 0;
}
/*****************************************************************************
 *
 *	sense interrupt status routine
 *
 *****************************************************************************/
int sis(
	struct unit_info *uip)
{
	register int	rtn, st0;

	if ((rtn = fdc_sts(FD_OSTS, uip)) != 0)	/* status check */
		return rtn;
	outb(DATAREG(uip->addr), SISCMD);
	if ((rtn = fdc_sts(FD_ISTS, uip)) != 0)	/* status check */
		return rtn;
	st0 = inb(DATAREG(uip->addr)) & ST0OK;	/* get st0 */
	if ((rtn = fdc_sts(FD_ISTS, uip)) != 0)	/* status check */
		return rtn;
	inb(DATAREG(uip->addr));	/* get pcn */
	if (st0&(ST0AT|ST0IC))
		st0 = FDCERR;
	return st0;
}

/*****************************************************************************
 *
 *	fdc status get routine
 *
 *****************************************************************************/
int fdc_sts(
	register int	mode,
	struct unit_info *uip)
{
	register int 	ind;
	int		cnt0 = STSCHKCNT;

	while(cnt0--)
		if(((ind=inb(STSREG(uip->addr))) & DATAOK) && 
		   ((ind & DTOCPU) == mode))
			return 0;
	return TIMEOUT;
}
/*****************************************************************************
 *
 *	motor on routine
 *
 *****************************************************************************/
void mtr_on(
	struct unit_info *uip)
{
	struct	fdcmd	*cmdp = uip->b_cmd;

	if (!mtr_start(uip)) {
		assert_wait(&cmdp->c_stsflag, FALSE);
		thread_set_timeout(1000);	/* 1 second */
		thread_block(CONTINUE_NULL);
	}
	cmdp->c_stsflag |= MTROFF;
	timeout(mtr_off,uip,MTRSTOP);
}
/*****************************************************************************
 *
 *	motor start routine
 *
 *****************************************************************************/
int mtr_start(
	struct unit_info *uip)
{
	int		status;
	struct	fdcmd	*cmdp = uip->b_cmd;
	int		slave = uip->slave;
	if(cmdp->c_stsflag & MTROFF){
		untimeout(mtr_off, uip);
		cmdp->c_stsflag &= ~MTROFF;
	}
	status = cmdp->c_rbmtr&(1<<slave);
	cmdp->c_rbmtr |= (1<<slave);
	outb(CTRLREG(uip->addr), ((cmdp->c_rbmtr&MTRMASK)<<MTR_ON)|
				     FDC_RST|slave|DMAREQ);
	return status;
}
/*****************************************************************************
 *
 *	motor off routine
 *
 *****************************************************************************/
void mtr_off(
	struct unit_info *uip)
{
	struct	fdcmd	*cmdp = uip->b_cmd;

	cmdp->c_stsflag &= ~MTROFF;
	if(!(cmdp->c_stsflag&MTRFLAG)){
		cmdp->c_rbmtr &= MTRRST;
		outb(CTRLREG(uip->addr), FDC_RST | DMAREQ);
	}
}