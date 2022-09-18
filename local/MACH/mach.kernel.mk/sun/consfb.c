/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	consfb.c,v $
 * Revision 2.4  89/03/09  21:29:34  rpd
 * 	More cleanup.
 * 
 * Revision 2.3  89/02/25  19:27:39  gm0w
 * 	Removed else legs of MACH and CMU conditionals and fixed 
 * 	includes as part of kernel cleanup.
 * 	[89/02/08            jjc]
 * 
 * 23-Oct-86  David Golub (dbg) at Carnegie-Mellon University
 *	Removed import of pte.h for MACH.
 *
 */

#ifndef	lint
static	char sccsid[] = "@(#)consfb.c 1.1 86/02/03 Copyr 1983 Sun Micro";
#endif

/*
 * Copyright (c) 1983 by Sun Microsystems, Inc.
 */

/*
 * Console frame buffer driver for Sun.
 *
 * Indirects to fbdev found during autoconf.
 */

#include <sys/param.h>
#include <sys/conf.h>
#include <sys/user.h>
#include <sys/ioctl.h>
#include <sys/tty.h>
#include <sys/systm.h>
#include <sys/kernel.h>
#include <sys/buf.h>
#include <sys/vmmac.h>
#include <sys/uio.h>
#include <sys/proc.h>

dev_t	fbdev = -1;

/* 
 * In case /dev/fb is indirected to a gp device (or any device that
 * may use the pseudo-minor device opening hack), we have to deal
 * specially with the minor device number.
 */

/*ARGSUSED*/
consfbopen(dev, flag, trueminor)
	dev_t dev;
	int *trueminor;
{
        register struct cdevsw *dp;

	if (fbdev == -1)
		return (ENXIO);
	dp = &cdevsw[major(fbdev)];
        return ((*dp->d_open)(fbdev, flag, trueminor));
}

/*ARGSUSED*/
consfbclose(dev, flag)
	dev_t dev;
{

	if (fbdev != -1)
		return ((*cdevsw[major(fbdev)].d_close)
			( (major(fbdev)<<8) | minor(dev), flag));
	return (-1);
}

/*ARGSUSED*/
consfbioctl(dev, cmd, data, flag)
	dev_t dev;
	caddr_t data;
{

	if (fbdev != -1)
                return ((*cdevsw[major(fbdev)].d_ioctl)
			( (major(fbdev)<<8) | minor(dev), cmd, data, flag));
	return (ENOTTY);
}

/*ARGSUSED*/
consfbmmap(dev, off, prot)
	dev_t dev;
	off_t off;
	int prot;
{
	if (fbdev != -1)
                return ((*cdevsw[major(fbdev)].d_mmap)
			( (major(fbdev)<<8) | minor(dev), off, prot));
	return (-1);
}
