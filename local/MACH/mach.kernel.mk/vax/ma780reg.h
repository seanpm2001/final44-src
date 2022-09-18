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
 * $Log:	ma780reg.h,v $
 * Revision 2.3  89/03/09  22:18:24  rpd
 * 	More cleanup.
 * 
 * Revision 2.2  89/02/25  20:24:35  gm0w
 * 	Changes for cleanup.
 * 
 */

/*CMU:	ma780reg.h	7.2	6/12/84	*/


/*	ma7800reg.h			3/28/84 */

/*
 * VAX MULTIPORT MEMORY controller registers
 */

struct mpm_regs
{
	u_long	mpm_csr;		/* configuration register */
	u_long	mpm_cr;			/* control register */
	u_long	mpm_sr;			/* controller status register */
	u_long	mpm_invld;		/* invalidation control register */
	u_long	mpm_err;		/* array error register */
	u_long	mpm_sr0;		/* configuration status register 0 */
	u_long	mpm_sr1;		/* configuration status register 1 */
	u_long	mpm_maint;		/* maintainance control register */
	u_long	mpm_ir;			/* interrupt request/status */
	u_long	mpm_en;			/* interrupt enable/status */
} ;


		/* configuration register */

#define MP_PWRUP	0x00400000
#define MP_PWRDWN	0x00800000
#define MP_XMTDUR	0x04000000
#define MP_MULXMT	0x08000000
#define MP_INTSEQ	0x10000000
#define MP_WRTSEQ	0x40000000
#define MP_PTY		0x80000000

#define MPCSR_BITS	"\
\20\40PTYFLT\37WRTSET\35INTLKSEQ\34MULTXMT\33\XMTDUR\
\30PWRDWN\27PWRUP\
\02\01PORT="

#define MP_PORT		0x00000003
#define MP_TYPE		0x000000ff

#define PWR_UP (1<<22)
#define PWR_DN (1<<23)

			/* control register */

#define MPCR_MSTRIE	0x00000001
#define MPCR_PIERR	0x00000002
#define MPCR_INHBRAM	0x00000010
#define MPCR_OUTPTYINV	0x00000020
#define MPCR_INPTYINV	0x00000040
#define MPCR_MARKINLK	0x00800000
#define MPCR_MRKTMO	0x01000000
#define MPCR_MRKREQ	0x02000000
#define MPCR_RAMOV	0x04000000
#define MPCR_IVDTACK	0x08000000
#define MPCR_RAMPAR	0x10000000
#define MPCR_IVDTLOST	0x20000000
#define MPCR_PTYFLTOUT	0x40000000
#define MPCR_PTYFLTIN	0x80000000

#define MPCR_BITS "\
\20\40PTYFLTIN\37PTYFLTOUT\36IVDTLOST\35RAMERR\34IVDTACK\33RAMOV\32MRKREQ\
\31MRKTMO\30MARKINLK\
\24\21RAMCNT=\
\07INPTYINV\06OUTPTYINV\05INHBRAM\02PIERRINTR\01MSTRIE"

#define MPCR_RAMCNT	0x000f0000

			/* controller status register */

#define MPSR_ERRIE	0x00000002
#define MPSR_ADMIPTY	0x00000010
#define MPSR_FRCXFLT	0x00000020
#define MPSR_SLFINVALIE	0x00000040
#define MPSR_INVALDIS	0x00000080
#define MPSR_INHBADMI	0x00000100
#define MPSR_INLKGRNT	0x00000400
#define MPSR_INLKFF	0x00000800
#define MPSR_ARYINIT	0x00001000
#define MPSR_SNGLSTEP	0x00002000
#define MPSR_LOSTINMPC	0x00004000
#define MPSR_INTLKTMO	0x00008000
#define MPSR_NOCAADMI	0x00200000
#define MPSR_EXADMI	0x00400000
#define MPSR_ARRHIOL	0x00800000
#define MPSR_CACHDADMI	0x01000000
#define MPSR_WRTHRDL	0x02000000
#define MPSR_ADMIGRNT	0x10000000
#define MPSR_XMTDURFLT	0x20000000
#define MPSR_MLTXMTRFL	0x40000000
#define MPSR_CMDABRT	0x80000000

#define MPSR_BITS	"\
\20\40CMDABRT\37MLTXMTRFL\36XMTDURFLT\35ADMIGRNT\
\34\33ADMIID=\
\32WRTHRDL\31CACHDADMI\30ARRHIOL\
\27EXADMI\26NOCAADMI\20INTLKTMO\17LOSTINMPC\16SNGLSTEP\15ARYINIT\14INLKFF\
\13INLKGRNT\11INHBADMI\10INVALDIS\07SLFINVALIE\06FRCXFLT\05ADMIPTY\03ERRIE"

#define MPSR_ID		0x0C000000

		/* invalidation control register */

#define MPINVLD_CACHEFORCED 0x80000000

#define MPINVLD_BITS "\
\20\40CACHEFORCED\
\37\25ADDR=\
\23\21SIZE=\
\20\1CACHEID="

#define MPINVLD_CACHID	0x0000ffff
#define MPINVLD_SIZE	0x00070000
#define MPINVLD_STRT	0x7ff00000

#define MPADDR_MSK (0x7ff)
#define MPADDR_SFT 20

		/* array error register */

#define MPERR_ERLOG	0x100000000
#define MPERR_HIER	0x200000000
#define MPERR_ICRD	0x400000000
#define MPERR_IVDTPTY	0x800000000

#define MPERR_BITS "\
\20\40IVDTPTY\37\INHCRDTG\36\HIRATE\35\LOGREQ\
\33\11ADDR=\
\10\1ERRSYND="

#define MPERR_SYNDROM	0x000000ff
#define MPERR_ADDR	0x07ffff00

		/* configuration status register 0 */

#define MPSR0_NONCNTGERRARR 0x00000001
#define MPSR0_4KCHPARRERR   0x00000002
#define MPSR0_PWRDN0	    0x00000010
#define MPSR0_PWRDN1	    0x00000020
#define MPSR0_PWRDN2	    0x00000040
#define MPSR0_PWRDN3	    0x00000080
#define MPSR0_ERRINT0	    0x00000800
#define MPSR0_ERRINT1	    0x00000400
#define MPSR0_ERRINT2	    0x00000200
#define MPSR0_ERRINT3	    0x00000100
#define MPSR0_OFFLINE0	    0x00001000
#define MPSR0_OFFLINE1	    0x00002000
#define MPSR0_OFFLINE2	    0x00004000
#define MPSR0_OFFLINE3	    0x00008000

#define MPSR0_BITS "\
\20\20OFFLINE3\17OFFLINE2\16OFFLINE1\15OFFLINE0\
\14ERRINT0\13ERRINT1\12ERRINT2\11ERRINT3\
\10PWRDN3\07PWRDN2\06PWRDN1\05PWRDN1\
\04\03ARRINT=\
\02_4KCHPARRERR\01NONCNTGERRARR"

#define MPSR0_INITSTAT	0x000000c0
#define MPSR0_PWRSTAT	0x00000f00
#define MPSR0_PORTERR	0x0000f000
#define MPSR0_PORTOFFL	0x000f0000

		/* configuration status register 1 */

#define MPSR1_FRCSLWNLK	0x00000100
#define MPSR1_FRCMLTNLK	0x00000200
#define MPSR1_MLTNTLKAC	0x00000400
#define MPSR1_IVDTMPRES	0x00000800
#define MPSR1_P0NVLDACC	0x00001000
#define MPSR1_P1NVLDACC	0x00002000
#define MPSR1_P2NVLDACC	0x00004000
#define MPSR1_P3NVLDACC	0x00008000

#define MPSR1_BITS "\
\20\20P3NVLDACC\17P2NVLDACC\16P1NVLDACC\15P0NVLDACC\
\14IVDTMPRES\13MLTNTLKAC\12FRCMLTNLK\11FRCSLWNLK\
\10\07TYP3=\06\05TYP2=\04\03TYP1=\02\01TYP0="

#define MPSR1_P0TYPE	0x00000003
#define MPSR1_P1TYPE	0x0000000c
#define MPSR1_P2TYPE	0x00000030
#define MPSR1_P3TYPE	0x000000c0

		/* maintainance control register */

#define MPMAINT_ECCBYPS	0x00000100
#define MPMAINT_FRCECC	0x00000200
#define MPMAINT_SLWADMI	0x00000400
#define MPMAINT_ADMIPAR	0x00000800
#define MPMAINT_ARRACCD	0x00001000
#define MPMAINT_IVDTPTY	0x00002000

#define MPMAINT_BITS	"\
\20\20\17NO=\
\16IVDTPTY\15ARRACCD\14ADMIPAR\13SLWADMI\12FRCECC\11ECCBYPS\
\10\01ECC="

#define MPMAINT_ECC	0x000000ff
#define MPMAINT_PORT	0x0000c000

		/* interrupt request/status */

#define MPISTAT_TO0FROM0	0x00000001
#define MPISTAT_TO0FROM1	0x00000002
#define MPISTAT_TO0FROM2	0x00000004
#define MPISTAT_TO0FROM3	0x00000008

#define MPISTAT_TO1FROM0	0x00000010
#define MPISTAT_TO1FROM1	0x00000020
#define MPISTAT_TO1FROM2	0x00000040
#define MPISTAT_TO1FROM3	0x00000080

#define MPISTAT_TO2FROM0	0x00000100
#define MPISTAT_TO2FROM1	0x00000200
#define MPISTAT_TO2FROM2	0x00000400
#define MPISTAT_TO2FROM3	0x00000800

#define MPISTAT_TO3FROM0	0x00001000
#define MPISTAT_TO3FROM1	0x00002000
#define MPISTAT_TO3FROM2	0x00004000
#define MPISTAT_TO3FROM3	0x00008000

#define MPISTAT_BITS "\
\20\20IR_3_FROM3\17IR_3_FROM2\16IR_3_FROM1\15IR_3_FROM0\
\14IR_2_FROM3\13IR_2_FROM2\12IR_2_FROM1\11IR_2_FROM0\
\10IR_1_FROM3\07IR_1_FROM2\06IR_1_FROM1\05IR_1_FROM0\
\04IR_0_FROM3\03IR_0_FROM2\02IR_0_FROM1\01IR_0_FROM0"

#define MPIR_TO0FROM0	0x00010000
#define MPIR_TO1FROM0	0x00020000
#define MPIR_TO2FROM0	0x00040000
#define MPIR_TO3FROM0	0x00080000

#define MPIR_TO0FROM1	0x00100000
#define MPIR_TO1FROM1	0x00200000
#define MPIR_TO2FROM1	0x00400000
#define MPIR_TO3FROM1	0x00800000

#define MPIR_TO0FROM2	0x01000000
#define MPIR_TO1FROM2	0x02000000
#define MPIR_TO2FROM2	0x04000000
#define MPIR_TO3FROM2	0x08000000

#define MPIR_TO0FROM3	0x10000000
#define MPIR_TO1FROM3	0x20000000
#define MPIR_TO2FROM3	0x40000000
#define MPIR_TO3FROM3	0x80000000

		/* interrupt enable/status */

#define MPESTAT_TO0FROM0	0x00000001
#define MPESTAT_TO0FROM1	0x00000002
#define MPESTAT_TO0FROM2	0x00000004
#define MPESTAT_TO0FROM3	0x00000008

#define MPESTAT_TO1FROM0	0x00000010
#define MPESTAT_TO1FROM1	0x00000020
#define MPESTAT_TO1FROM2	0x00000040
#define MPESTAT_TO1FROM3	0x00000080

#define MPESTAT_TO2FROM0	0x00000100
#define MPESTAT_TO2FROM1	0x00000200
#define MPESTAT_TO2FROM2	0x00000400
#define MPESTAT_TO2FROM3	0x00000800

#define MPESTAT_TO3FROM0	0x00001000
#define MPESTAT_TO3FROM1	0x00002000
#define MPESTAT_TO3FROM2	0x00004000
#define MPESTAT_TO3FROM3	0x00008000

#define MPESTAT_BITS "\
\20\20EN_3_FROM3\17EN_3_FROM2\16EN_3_FROM1\15EN_3_FROM0\
\14EN_2_FROM3\13EN_2_FROM2\12EN_2_FROM1\11EN_2_FROM0\
\10EN_1_FROM3\07EN_1_FROM2\06EN_1_FROM1\05EN_1_FROM0\
\04EN_0_FROM3\03EN_0_FROM2\02EN_0_FROM1\01EN_0_FROM0"

#define MPEN_TO0FROM0	0x00010000
#define MPEN_TO0FROM1	0x00020000
#define MPEN_TO0FROM2	0x00040000
#define MPEN_TO0FROM3	0x00080000

#define MPEN_TO1FROM0	0x00100000
#define MPEN_TO1FROM1	0x00200000
#define MPEN_TO1FROM2	0x00400000
#define MPEN_TO1FROM3	0x00800000

#define MPEN_TO2FROM0	0x01000000
#define MPEN_TO2FROM1	0x02000000
#define MPEN_TO2FROM2	0x04000000
#define MPEN_TO2FROM3	0x08000000

#define MPEN_TO3FROM0	0x10000000
#define MPEN_TO3FROM1	0x20000000
#define MPEN_TO3FROM2	0x40000000
#define MPEN_TO3FROM3	0x80000000
