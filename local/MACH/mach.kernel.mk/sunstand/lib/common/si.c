/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	si.c,v $
 * Revision 2.2  89/07/12  00:20:35  jjc
 * 	Picked up from SunOS 4.0.
 * 	[89/07/03            jjc]
 * 
 */
/*
 * @(#)si.c 1.10 88/02/08 Copyright (c) 1985 by Sun Microsystems, Inc.
 */

#include <sys/types.h>
#include <sys/buf.h>
#include <sun/dklabel.h>
#include <sun/dkio.h>
#include <stand/screg.h>
#include <stand/sireg.h>
#include <stand/scsi.h>
#include <stand/saio.h>
#include <mon/sunromvec.h>
#include <mon/idprom.h>

/*
 * Low-level routines common to all devices on the SCSI bus.
 */

/*
 * Interface to the routines in this module is via a second "sip"
 * structure contained in the caller's local variables.
 *
 * This "sip" must be initialized with sip->si_boottab = sidriver
 * and must then be devopen()ed before any I/O can be done.
 *
 * The device must be closed with devclose().
 */


/* how si addresses look to the si vme scsi dma hardware */
#ifdef	NOT
#define	SI_VME_DMA_ADDR(x)	(((int)x)&0x000F)
#else
#define	SI_VME_DMA_ADDR(x)	(((int)x)&0x000FFFFF)
#endif

/* how si addresses look to the sun3/50 scsi dma hardware */
#define	SI_OB_DMA_ADDR(x)	(((int)x)&0x00FFFFFF)

/* how si addresses look to the sun4/110 scsi dma hardware */
#define	SI_COBRA_DMA_ADDR(x)	(((int)x)&0x00FFFFFF)

struct sidma {
	struct udc_table	udct;	/* dma information for udc */
};

/*
 * The interfaces we export
 */
extern char *devalloc();
extern char *resalloc();
extern int nullsys();
int siopen();
int si_type;
int junk;

#define COBRA 1
#define M25 2
#define VME 3

#define ISCOBRA()	(si_type == COBRA)
struct boottab sidriver =
	{"sd",	nullsys,	nullsys,	siopen,		nullsys,
		nullsys,	"",	0};


#define SI_COBRA_BASE	0xfa000000
#define SI_VME_BASE	0x200000
#define SI_OB_BASE	0x140000
#define SI_SIZE		0x4000

/*
 * Probe for si host adaptor interface. 
 * Return 1 if found one, 0 otherwise.
 */
int
siprobe(sip)
	register struct saioreq	*sip;
{
	register struct scsi_si_reg *sir;
	struct idprom id;
	register int base;
	register int scsi_nstd;
	register int ctlr;
	enum MAPTYPES space;

	/* determine type of si interface */
	if (idprom(IDFORM_1, &id) == IDFORM_1) {
		if (id.id_machine == IDM_SUN3_M25 ||
		    id.id_machine == IDM_SUN4_COBRA ||
		    id.id_machine == IDM_SUN3_F) {
			return (1);
		} else {
			base = SI_VME_BASE;
			space = MAP_VME24A16D;
			scsi_nstd = 2;
		}
	} else {
		return (0);
	}

	/* get base address of registers */
	if (sip->si_ctlr < scsi_nstd) {
		ctlr = base + (sip->si_ctlr * SI_SIZE);
	} else {
		return (0);
	}

	/* now map it in */
	if ((sir = (struct scsi_si_reg *)devalloc(space, ctlr, 
	    sizeof(struct scsi_si_reg))) == 0) {
		return (0);
	}
	
	/*
	 * SI vme scsi host adaptor occupies 2K bytes in the vme 
	 * address space. SC vme scsi host adaptor occupies 4K 
	 * bytes in the vme address space.
	 * peek past 2K bytes to determine which host adaptor is there.
	 */
	if (peek((int)sir+0x800) == -1) {
		return (1);
	} else {
		return (0);
	}
}

/*
 * Open the SCSI host adapter.
 */
int
siopen(sip)
	register struct saioreq	*sip;
{
	register struct scsi_si_reg *sir;
	register struct scsi_sw_reg *swr;
	struct idprom id;
	register int base;
	register int scsi_nstd;
	enum MAPTYPES space;

	/* determine type of si interface */
	/* determine type of si interface */
	if (idprom(IDFORM_1, &id) == IDFORM_1) {
		switch (id.id_machine) {
		case IDM_SUN4_COBRA:
			base = SI_COBRA_BASE;
			space = MAP_OBIO;
			scsi_nstd = 1;
			si_type = COBRA;
			break;
		case IDM_SUN3_F:
		case IDM_SUN3_M25:
			base = SI_OB_BASE;
			space = MAP_OBIO;
			scsi_nstd = 1;
			si_type = M25;
			break;
	        default:
			base = SI_VME_BASE;
			space = MAP_VME24A16D;
			scsi_nstd = 2;
			si_type = VME;
			break;
		}
	} else {
		return (-1);
	}

	/* get base address of registers */
	if (sip->si_ctlr < scsi_nstd) {
		sip->si_ctlr = base + (sip->si_ctlr * SI_SIZE);
	} else {
		return (-1);
	}

	/* now map it in */
	sip->si_devaddr = devalloc(space, sip->si_ctlr, 
	    sizeof(struct scsi_si_reg));
	if (sip->si_devaddr == 0) {
		return (-1);
	}

	/* allocate dma resources */
	if (base == SI_OB_BASE) {
		sip->si_dmaaddr = resalloc(RES_DMAMEM, sizeof(struct sidma));
		if (sip->si_dmaaddr == 0) {
			return (-1);
		}
	} else {
		sip->si_dmaaddr = 0;
	}

	/* reset registers */
	if (base == SI_COBRA_BASE) {
		swr = (struct scsi_sw_reg *) sip->si_devaddr;
		swr->csr = 0;
		DELAY(10);
		swr->csr = SI_CSR_SCSI_RES;
		swr->dma_addr = 0;
		swr->dma_count = 0;
	} else {
		sir = (struct scsi_si_reg *) sip->si_devaddr;
		sir->csr = 0;
		DELAY(10);
		sir->csr = SI_CSR_SCSI_RES | SI_CSR_FIFO_RES;
		sir->bcr = 0;
		if (base == SI_VME_BASE) {
			sir->dma_addrl = 0;
			sir->dma_addrh = 0;
			sir->dma_countl = 0;
			sir->dma_counth = 0;
			sir->iv_am = VME_SUPV_DATA_24;
		}
	}
	return (0);
}


/*
 * Write a command to the SCSI bus.
 *
 * The supplied sip is the one opened by siopen().
 * DMA is done based on sip->si_ma and sip->si_cc.
 *
 * Returns -1 for error, otherwise returns the residual count not DMAed
 * (zero for success).
 *
 * FIXME, this must be accessed via a boottab vector,
 * to allow host adap to switch.
 * Must pass cdb, scb in sip somewhere...
 */
int
sidoit(cdb, scb, sip)
	struct scsi_cdb *cdb;
	struct scsi_scb *scb;
	register struct saioreq *sip;
{
	register struct scsi_si_reg *sir;
	register struct scsi_sw_reg *swr;
	register char *cp;
	register int i;
	register int b;
	register int ob;
	u_char junk;

	/* get to scsi control logic registers */
	if (ISCOBRA())
		swr = (struct scsi_sw_reg *) sip->si_devaddr;
	else
		sir = (struct scsi_si_reg *) sip->si_devaddr;
	if (sip->si_dmaaddr) {
		ob = 1;
	} else {
		ob = 0;
	}

	i = 100000;
	for(;;) {
		if (ISCOBRA()) {
			if ((swr->sbc_rreg.cbsr & SBC_CBSR_BSY) == 0) {
				break;
			}
		} else {
			if ((sir->sbc_rreg.cbsr & SBC_CBSR_BSY) == 0) {
				break;
			}
		}
		if (i-- <= 0) {
			if (ISCOBRA())
				si_reset(swr, ob);
			else
				si_reset(sir, ob);
			return (-1);
		}
	}

	/* select target */
	if (ISCOBRA()) {
		swr->sbc_wreg.odr = (1 << sip->si_unit) | SI_HOST_ID;
		swr->sbc_wreg.icr = SBC_ICR_DATA;
		swr->sbc_wreg.icr |= SBC_ICR_SEL;
	} else {
		sir->sbc_wreg.odr = (1 << sip->si_unit) | SI_HOST_ID;
		sir->sbc_wreg.icr = SBC_ICR_DATA;
		sir->sbc_wreg.icr |= SBC_ICR_SEL;
	}
	/* wait for target to acknowledge our selection */
	if (ISCOBRA()) {
		if (si_sbc_wait(&swr->sbc_rreg.cbsr, SBC_CBSR_BSY, 1) == 0) {
			sc_error("sel, cbsr bsy never set");
			goto failed;
		}
		swr->sbc_wreg.icr = 0;
	} else {
		if (si_sbc_wait(&sir->sbc_rreg.cbsr, SBC_CBSR_BSY, 1) == 0) {
			sc_error("sel, cbsr bsy never set");
			goto failed;
		}
		sir->sbc_wreg.icr = 0;
		
	/* do initial dma setup */
		sir->bcr = 0;	/* also reset dma_count for vme */
	}
	if (sip->si_cc > 0) {
		if ((cdb->scc_cmd == SC_READ) ||
		       (cdb->scc_cmd == SC_REQUEST_SENSE)) {
			if (ISCOBRA())
				swr->csr &= ~SI_CSR_SEND;
			else
				sir->csr &= ~SI_CSR_SEND;
		} else {
			if (ISCOBRA())
				swr->csr |= SI_CSR_SEND;
			else
				sir->csr |= SI_CSR_SEND;
		}
		if (!ISCOBRA()) {
			sir->csr &= ~SI_CSR_FIFO_RES;
			sir->csr |= SI_CSR_FIFO_RES;	
			sir->bcr = sip->si_cc;
			sir->dma_countl = ADDR_LO(0);
			sir->dma_counth = ADDR_HI(0);
			if (ob == 0)
				sir->bcrh = 0;
		}
	}

	/* put command onto scsi bus */
	cp = (char *)cdb;
	if (ISCOBRA()) {
		if (si_putbyte(swr, PHASE_COMMAND, cp, SC_GROUP_0_SIZE) == 0) {
			sc_error("put of cmd onto scsi bus failed");
			goto failed;
		}
	} else {
		if (si_putbyte(sir, PHASE_COMMAND, cp, SC_GROUP_0_SIZE) == 0) {
			sc_error("put of cmd onto scsi bus failed");
			goto failed;
		}
	}
	/* finish dma setup and wait for dma completion */
	if (sip->si_cc > 0) {
		if (ob) {
#ifndef sun4
		    si_ob_dma_setup(sir, 
			&(((struct sidma *)sip->si_dmaaddr)->udct), 
			cdb->scc_cmd, sip->si_cc, sip->si_ma);
#endif
		} else {
			if (ISCOBRA()) {
				if (((int)sip->si_ma & 1) ||
					((int)sip->si_ma & 2)) {
					sc_error("dma begins on odd address");
					goto failed;
				}
				swr->dma_addr = SI_COBRA_DMA_ADDR(sip->si_ma);
				swr->dma_count = sip->si_cc;
			} else {
				if ((int)sip->si_ma & 1) {
					sc_error("dma begins on odd address");
					goto failed;
				} else if ((int)sip->si_ma & 2) {
					sir->csr |= SI_CSR_BPCON;
				} else {
					sir->csr &= ~SI_CSR_BPCON;
				}
				sir->dma_addrl =
					ADDR_LO(SI_VME_DMA_ADDR(sip->si_ma));
				sir->dma_addrh = 
					ADDR_HI(SI_VME_DMA_ADDR(sip->si_ma));
				sir->dma_countl = ADDR_LO(sip->si_cc);
				sir->dma_counth = ADDR_HI(sip->si_cc);
			}
		}

		if (ISCOBRA())
			si_cobra_dma_setup(sip, swr, ob, cdb);
		else
			si_dma_setup(sip, sir, ob, cdb);
	}
	/* get status */
	cp = (char *)scb;
	for (i = 0;;) {
		if (ISCOBRA())
			b = si_getbyte(swr, PHASE_STATUS);
		else
			b = si_getbyte(sir, PHASE_STATUS);
		if (b == -1) {
			break;
		}
		if (i < STATUS_LEN) {
			cp[i++] = b;
		}
	}
	if (ISCOBRA())
		b = si_getbyte(swr, PHASE_MSG_IN);
	else
		b = si_getbyte(sir, PHASE_MSG_IN);
	if (b != SC_COMMAND_COMPLETE) {
		if (b >= 0) {	/* if not, si_getbyte already printed msg */
			sc_error("invalid message");
		}
		goto failed;
	}
	if (ISCOBRA())
		return (sip->si_cc);
	else
		return (sip->si_cc - sir->bcr);

failed:
	if (ISCOBRA())
		si_reset(swr, ob);
	else
		si_reset(sir, ob);
	return (-1);
}

si_cobra_dma_setup(sip, swr, ob, cdb)
register struct saioreq	*sip;
register struct scsi_sw_reg *swr;
int ob;
struct scsi_cdb *cdb;
{       
	register char *cp;

	/* setup sbc and start dma */
	swr->sbc_wreg.mr |= SBC_MR_DMA;
	if ((cdb->scc_cmd == SC_READ) || (cdb->scc_cmd == SC_REQUEST_SENSE)) {
		swr->sbc_wreg.tcr = TCR_DATA_IN;
		swr->sbc_wreg.ircv = 0;
	} else {
		swr->sbc_wreg.tcr = TCR_DATA_OUT;
		swr->sbc_wreg.icr = SBC_ICR_DATA;
		swr->sbc_wreg.send = 0;
	}
	swr->csr |= SI_CSR_DMA_EN;

	/* wait for dma completion */
	if (si_waitl(&swr->csr, 
		SI_CSR_SBC_IP|SI_CSR_DMA_CONFLICT|SI_CSR_DMA_BUS_ERR, 1) == 0) {
		sc_error("dma never completed");
		swr->csr &= ~SI_CSR_DMA_EN;
		si_dma_cleanup(swr, ob);
		goto failed;
	}
	
		/* check reason for dma completion */
	if (swr->csr & SI_CSR_SBC_IP) {
			/* dma operation should end with a phase mismatch */
		swr->csr &= ~SI_CSR_DMA_EN;
		si_sbc_wait(&swr->sbc_rreg.bsr, SBC_BSR_PMTCH, 0);
	} else {
		swr->csr &= ~SI_CSR_DMA_EN;
		if (swr->csr & SI_CSR_DMA_CONFLICT) {
			sc_error("invalid reg access during dma");
		} else if (swr->csr & SI_CSR_DMA_BUS_ERR) {	
			sc_error("bus error during dma");
		} else {
			sc_error("dma overrun");
		}
		si_dma_cleanup(swr, ob);
		goto failed;
	}

		/* handle special dma recv situations */
	if ((cdb->scc_cmd == SC_READ) || (cdb->scc_cmd == SC_REQUEST_SENSE)) {
		cp = (char *)((swr->dma_addr&0xFFFFFC)|0xff000000);
		switch (swr->dma_addr & 0x3) {
		    case 3:
			    *cp = (swr->bpr & 0xff000000) >> 24;
			    *(cp + 1) = (swr->bpr & 0x00ff0000) >> 16;
			    *(cp + 2) = (swr->bpr & 0x0000ff00) >> 8;
			    break;
		    case 2:
			    *cp = (swr->bpr & 0xff000000) >> 24;
			    *(cp + 1) = (swr->bpr & 0x00ff0000) >> 16;
			    break;
		    case 1:
			    *cp = (swr->bpr & 0xff000000) >> 24;
			    break;
		}
	}

		/* clear sbc interrupt */
	junk = swr->sbc_rreg.clr;
#ifdef lint
	junk = junk;		/* use it */
#endif
	
		/* cleanup after a dma operation */
	si_dma_cleanup(swr, ob);
	return(0);
failed:
	return(-1);

}

si_dma_setup(sip, sir, ob, cdb)
register struct saioreq	*sip;
register struct scsi_si_reg *sir;
int ob;
struct scsi_cdb *cdb;
{       
	register char *cp;

	sir->bcr = sip->si_cc;
	sir->dma_countl = ADDR_LO(sip->si_cc);
	sir->dma_counth = ADDR_HI(sip->si_cc);
	/* setup sbc and start dma */
	sir->sbc_wreg.mr |= SBC_MR_DMA;
	if ((cdb->scc_cmd == SC_READ) || (cdb->scc_cmd == SC_REQUEST_SENSE)) {
		sir->sbc_wreg.tcr = TCR_DATA_IN;
		sir->sbc_wreg.ircv = 0;
	} else {
		sir->sbc_wreg.tcr = TCR_DATA_OUT;
		sir->sbc_wreg.icr = SBC_ICR_DATA;
		sir->sbc_wreg.send = 0;
	}
	if (ob == 0) {
		sir->csr |= SI_CSR_DMA_EN;
	}

	/* wait for dma completion */
	if (si_wait(&sir->csr, 
		       SI_CSR_SBC_IP|SI_CSR_DMA_IP|SI_CSR_DMA_CONFLICT, 1)
	       == 0) {
		sc_error("dma never completed");
		if (ob == 0) {
			sir->csr &= ~SI_CSR_DMA_EN;
		}
		si_dma_cleanup(sir, ob);
		goto failed;
	}
	if (ob == 0) {
		sir->csr &= ~SI_CSR_DMA_EN;
	}
	
		/* check reason for dma completion */
	if (sir->csr & SI_CSR_SBC_IP) {
			/* dma operation should end with a phase mismatch */
		si_sbc_wait(&sir->sbc_rreg.bsr, SBC_BSR_PMTCH, 0);
	} else {
		if (sir->csr & SI_CSR_DMA_CONFLICT) {
			sc_error("invalid reg access during dma");
		} else if (sir->csr & SI_CSR_DMA_BUS_ERR) {	
			sc_error("bus error during dma");
		} else {
			if (ob)
				sc_error("unknown dma failure");
			else
				sc_error("dma overrun");
		}
		si_dma_cleanup(sir, ob);
		goto failed;
	}

		/* handle special dma recv situations */
	if ((cdb->scc_cmd == SC_READ) || (cdb->scc_cmd == SC_REQUEST_SENSE)) {
		if (ob) {
			sir->udc_raddr = UDC_ADR_COUNT;
			if (si_wait(&sir->csr, SI_CSR_FIFO_EMPTY, 1) == 0) {
				sc_error("fifo never emptied");
				si_dma_cleanup(sir, ob);
				goto failed;
			}
			/* if odd byte recv, must grab last byte by hand */
			if ((sip->si_cc - sir->bcr) & 1) {
				cp = sip->si_ma + (sip->si_cc - sir->bcr) - 1;
				*cp = (sir->fifo_data & 0xff00) >> 8;

			/* udc may not dma last word */
			} else if (((sir->udc_rdata*2) - sir->bcr) == 2) {
				cp = sip->si_ma + (sip->si_cc - sir->bcr);
				*(cp - 2) = (sir->fifo_data & 0xff00) >> 8;
				*(cp - 1) = sir->fifo_data & 0x00ff;
			}

		} else if ((sir->csr & SI_CSR_LOB) != 0) {
			cp = sip->si_ma + (sip->si_cc - sir->bcr);
			if ((sir->csr & SI_CSR_BPCON) == 0) {
			    switch (sir->csr & SI_CSR_LOB) {
			    case SI_CSR_LOB_THREE:
				    *(cp - 3) = (sir->bprh & 0xff00) >> 8;
				    *(cp - 2) = (sir->bprh & 0x00ff);
				    *(cp - 1) = (sir->bprl & 0xff00) >> 8;
				    break;
			    case SI_CSR_LOB_TWO:
				    *(cp - 2) = (sir->bprh & 0xff00) >> 8;
				    *(cp - 1) = (sir->bprh & 0x00ff);
				    break;
			    case SI_CSR_LOB_ONE:
				    *(cp - 1) = (sir->bprh & 0xff00) >> 8;
				    break;
			    }
			} else {
				*(cp - 1) = (sir->bprl & 0xff00) >> 8;
			}
		}
	}

		/* clear sbc interrupt */
	junk = sir->sbc_rreg.clr;
#ifdef lint
	junk = junk;		/* use it */
#endif
	
		/* cleanup after a dma operation */
	si_dma_cleanup(sir, ob);
	return(0);
failed:
        return(-1);
}

#ifndef sun4
si_ob_dma_setup(sir, udct, cmd, cc, ma)
	register struct scsi_si_reg *sir;
	register struct udc_table *udct;
	register u_char cmd;
	register int cc;
	register char *ma;
{
	/* setup udc dma info */
	udct->haddr = ((SI_OB_DMA_ADDR(ma) & 0xff0000) >> 8) | 
		UDC_ADDR_INFO;
	udct->laddr = SI_OB_DMA_ADDR(ma) & 0xffff;
	udct->hcmr = UDC_CMR_HIGH;
	udct->count = cc / 2;
	if ((cmd == SC_READ) || (cmd == SC_REQUEST_SENSE)) {
		udct->rsel = UDC_RSEL_RECV;
		udct->lcmr = UDC_CMR_LRECV;
	} else {
		udct->rsel = UDC_RSEL_SEND;
		udct->lcmr = UDC_CMR_LSEND;
		if (cc & 1) {
			udct->count++;
		}
	}

	/* initialize chain address register */
	DELAY(SI_UDC_WAIT);
	sir->udc_raddr = UDC_ADR_CAR_HIGH;
	DELAY(SI_UDC_WAIT);
	sir->udc_rdata = ((int)udct & 0xff0000) >> 8;
	DELAY(SI_UDC_WAIT);
	sir->udc_raddr = UDC_ADR_CAR_LOW;
	DELAY(SI_UDC_WAIT);
	sir->udc_rdata = (int)udct & 0xffff;

	/* initialize master mode register */
	DELAY(SI_UDC_WAIT);
	sir->udc_raddr = UDC_ADR_MODE;
	DELAY(SI_UDC_WAIT);
	sir->udc_rdata = UDC_MODE;

	/* issue start chain command */
	DELAY(SI_UDC_WAIT);
	sir->udc_raddr = UDC_ADR_COMMAND;
	DELAY(SI_UDC_WAIT);
	sir->udc_rdata = UDC_CMD_STRT_CHN;
}
#endif

/*
 * Reset some register information after a dma operation.
 */
si_dma_cleanup(sir, ob)
	register struct scsi_si_reg *sir;
	register int ob;
{
	register struct scsi_sw_reg *swr;

	if (ISCOBRA())
		swr = (struct scsi_sw_reg *)sir;
	if (ob) {
		sir->udc_raddr = UDC_ADR_COMMAND;
		DELAY(SI_UDC_WAIT);
		sir->udc_rdata = UDC_CMD_RESET;
	} else {
		if (ISCOBRA()) {
			swr->csr &= ~SI_CSR_DMA_EN;
			swr->dma_addr = 0;
		} else {
			sir->csr &= ~SI_CSR_DMA_EN;
			sir->dma_addrl = 0;
			sir->dma_addrh = 0;
		}
	}
	if (ISCOBRA()) {
		swr->sbc_wreg.mr &= ~SBC_MR_DMA;
		swr->sbc_wreg.icr = 0;
		swr->sbc_wreg.tcr = 0;
	} else {
		sir->sbc_wreg.mr &= ~SBC_MR_DMA;
		sir->sbc_wreg.icr = 0;
		sir->sbc_wreg.tcr = 0;
	}
}

/*
 * Wait for a condition to be (de)asswrted.
 */
si_wait(reg, cond, set)
	register u_short *reg;
	register u_short cond;
	register int set;
{
	register int i;
	register u_short regval;

	for (i = 0; i < 1000000; i++) {
		regval = *reg;
		if ((set == 1) && (regval & cond)) {
			return (1);
		}
		if ((set == 0) && !(regval & cond)) {
			return (1);
		} 
		DELAY(600);
	}
	return (0);
}

/*
 * Wait for a condition to be (de)asserted.
 */
si_waitl(reg, cond, set)
	register u_int *reg;
	register u_int cond;
	register int set;
{
	register int i;
	register u_int regval;

	for (i = 0; i < 1000000; i++) {
		regval = *reg;
		if ((set == 1) && (regval & cond)) {
			return (1);
		}
		if ((set == 0) && !(regval & cond)) {
			return (1);
		} 
		DELAY(600);
	}
	return (0);
}

/*
 * Wait for a condition to be (de)asserted on the scsi bus.
 */
si_sbc_wait(reg, cond, set)
	register caddr_t reg;
	register u_char cond;
	register int set;
{
	register int i;
	register u_char regval;

	for (i = 0; i < 1000000; i++) {
		regval = *reg;
		if ((set == 1) && (regval & cond)) {
			return (1);
		}
		if ((set == 0) && !(regval & cond)) {
			return (1);
		} 
		DELAY(600);
	}
	return (0);
}

/*
 * Put a byte onto the scsi bus.
 */
si_putbyte(sir, phase, data, numbytes)
	register struct scsi_si_reg *sir;
	register u_short phase;
	register u_char *data;
	register int numbytes;
{
	register int i;

	/* set up tcr so a phase match will occur */
	if (phase == PHASE_COMMAND) {
		sir->sbc_wreg.tcr = TCR_COMMAND;
	} else if (phase == PHASE_MSG_OUT) {
		sir->sbc_wreg.tcr = TCR_MSG_OUT;
	} else {
		sc_error("putbyte, bad phase specified");
		return (0);
	}

	/* put all desired bytes onto scsi bus */
	for (i = 0; i < numbytes; i++) {
		/* wait for target to request a byte */
		if (si_sbc_wait(&sir->sbc_rreg.cbsr, SBC_CBSR_REQ, 1) == 0) {
			sc_error("putbyte: target never set REQ");
			return (0);
		}

		/* load data for transfer */
		sir->sbc_wreg.odr = *data++;
		sir->sbc_wreg.icr = SBC_ICR_DATA;

		/* make sure phase match occurred */
		if ((sir->sbc_rreg.bsr & SBC_BSR_PMTCH) == 0) {
			sc_error("putbyte: phase mismatch");
			return (0);
		}
		/* complete req/ack handshake */
		sir->sbc_wreg.icr |= SBC_ICR_ACK;
		if (si_sbc_wait(&sir->sbc_rreg.cbsr, SBC_CBSR_REQ, 0) == 0) {
			sc_error("putbyte: target never released REQ");
			return (0);
		}
		sir->sbc_wreg.icr = 0;

	}
	sir->sbc_wreg.tcr = 0;
return (1);
}

/*
 * Get a byte from the scsi bus.
 */
si_getbyte(sir, phase)
	register struct scsi_si_reg *sir;
	register u_short phase;
{
	register u_char data;

	/* set up tcr so a phase match will occur */
	if (phase == PHASE_STATUS) {
		sir->sbc_wreg.tcr = TCR_STATUS;
	} else if (phase == PHASE_MSG_IN) {
		sir->sbc_wreg.tcr = TCR_MSG_IN;
	} else {
		sc_error("getbyte, bad phase specified");
		return (-1);
	}

	/* wait for target request */
	if (si_sbc_wait(&sir->sbc_rreg.cbsr, SBC_CBSR_REQ, 1) == 0) {
		sc_error("getbyte: target never set REQ");
		sir->sbc_wreg.tcr = 0;
		return (-1);
	}

	/* check for correct information phase on scsi bus */
	if (phase != (sir->sbc_rreg.cbsr & CBSR_PHASE_BITS)) {
		if (phase != PHASE_STATUS) {
			sc_error("getbyte: phase mismatch");
		}
		sir->sbc_wreg.tcr = 0;
		return (-1);
	}

	/* grab data */
	data = sir->sbc_rreg.cdr;
	sir->sbc_wreg.icr = SBC_ICR_ACK;

	/* complete req/ack handshake */
	if (si_sbc_wait(&sir->sbc_rreg.cbsr, SBC_CBSR_REQ, 0) == 0) {
		sc_error("getbyte: target never released REQ");
		sir->sbc_wreg.icr = 0;
		sir->sbc_wreg.tcr = 0;
		return (-1);
	}
	sir->sbc_wreg.icr = 0;
	sir->sbc_wreg.tcr = 0;
	return (data);
}

/*
 * Reset SCSI control logic.
 */
si_reset(sir, ob)
	register struct scsi_si_reg *sir;
	register int ob;
{
	register u_char junk;
	register struct scsi_sw_reg *swr = (struct scsi_sw_reg *)sir;

	/* reset bcr, fifo, udc, and sbc */
	if (ISCOBRA()) {
		swr->csr = 0;
		DELAY(10);
		swr->csr = SI_CSR_SCSI_RES;
		swr->dma_count = 0;
		swr->dma_addr = 0;
	}else {
		sir->bcr = 0;
		sir->csr = 0;
		DELAY(10);
		sir->csr = SI_CSR_SCSI_RES|SI_CSR_FIFO_RES;
	}
	if(!ISCOBRA() && (ob == 0)) {
		sir->dma_addrl = 0;
		sir->dma_addrh = 0;
		sir->dma_countl = 0;
		sir->dma_counth = 0;
	}

	/* issue scsi bus reset */
	if (ISCOBRA()) {
		swr->sbc_wreg.icr = SBC_ICR_RST;
		DELAY(1000);
		swr->sbc_wreg.icr = 0;
		junk = swr->sbc_rreg.clr;
		swr->sbc_wreg.mr = 0;
	}else {
		sir->sbc_wreg.icr = SBC_ICR_RST;
		DELAY(50);
		sir->sbc_wreg.icr = 0;
		junk = sir->sbc_rreg.clr;
	}
#ifdef lint
	junk = junk;		/* use it */
#endif
	DELAY(10000000);
}
