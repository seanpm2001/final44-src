/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 * HISTORY
 * $Log:	consms.c,v $
 * Revision 2.4  89/03/09  21:29:49  rpd
 * 	More cleanup.
 * 
 * Revision 2.3  89/02/25  19:27:53  gm0w
 * 	Removed else legs of MACH and CMU conditionals and fixed 
 * 	includes as part of kernel cleanup.
 * 	[89/02/08            jjc]
 * 
 * 23-Oct-86  David Golub (dbg) at Carnegie-Mellon University
 *	Removed import of pte.h for MACH.
 *
 */

#ifndef	lint
static	char sccsid[] = "@(#)consms.c 1.1 86/02/03 Copyr 1983 Sun Micro";
#endif

/*
 * Copyright (c) 1983 by Sun Microsystems, Inc.
 */

/*
 * Console mouse driver for Sun.
 *
 * Indirects to mousedev found during autoconf.
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

dev_t	mousedev = -1;

/*ARGSUSED*/
consmsopen(dev, flag)
	dev_t dev;
{
        int err;
        int ldisc = MOUSELDISC;
        register struct cdevsw *dp;

	if (mousedev == -1)
		return (ENXIO);
	dp = &cdevsw[major(mousedev)];
        if (err = (*dp->d_open)(mousedev, flag))
                return (err);
	if (err = (*dp->d_ioctl) (mousedev, TIOCSETD, (caddr_t)&ldisc, flag))
                return (err);
	cdevsw[major(dev)].d_ttys =
	    cdevsw[major(mousedev)].d_ttys+minor(mousedev);
	return(0);
}

/*ARGSUSED*/
consmsclose(dev, flag)
	dev_t dev;
{
	if (mousedev != -1) {
		(*cdevsw[major(mousedev)].d_close)(mousedev, flag);
		cdevsw[major(dev)].d_ttys = 0;
	}
}

/*ARGSUSED*/
consmsread(dev, uio)
	dev_t dev;
	struct uio *uio;
{

	if (mousedev != -1)
		return ((*cdevsw[major(mousedev)].d_read)(mousedev, uio));
	return (ENXIO);
}

/*ARGSUSED*/
consmsselect(dev, rw)
	dev_t dev;
	int rw;
{

	if (mousedev != -1)
		return ((*cdevsw[major(mousedev)].d_select)(mousedev, rw));
	return (ENXIO);
}

/*ARGSUSED*/
consmsioctl(dev, cmd, data, flag)
	dev_t dev;
	caddr_t data;
{

	if (mousedev != -1)
		return ((*cdevsw[major(mousedev)].d_ioctl)
		    (mousedev, cmd, data, flag));
	return (ENOTTY);
}
