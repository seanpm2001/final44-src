/*-
 * Copyright (c) 1992 Keith Muller.
 * Copyright (c) 1992, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Keith Muller of the University of California, San Diego.
 *
 * %sccs.include.redist.c%
 */

#ifndef lint
static char copyright[] =
"%Z% Copyright (c) 1992, 1993\n\
	The Regents of the University of California.  All rights reserved.\n";
#endif /* not lint */

#ifndef lint
static char sccsid[] = "%W% (Berkeley) %G%";
#endif /* not lint */

#include <stdio.h>
#include <sys/types.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include "pax.h"
#include "extern.h"
static int gen_init __P((void));

/*
 * PAX main routines, general globals and some simple start up routines
 */

/*
 * Variables that can be accessed by any routine within pax
 */
int	act = DEFOP;		/* read/write/append/copy */
FSUB	*frmt = NULL;		/* archive format type */
int	cflag;			/* match all EXCEPT pattern/file */
int	dflag;			/* directory member match only  */
int	iflag;			/* interactive file/archive rename */
int	kflag;			/* do not overwrite existing files */
int	lflag;			/* use hard links when possible */
int	nflag;			/* select first archive member match */
int	tflag;			/* restore access time after read */
int	uflag;			/* ignore older modification time files */
int	vflag;			/* produce verbose output */
int	Dflag;			/* same as uflag except inode change time */
int	Hflag;			/* follow command line symlinks (write only) */
int	Lflag;			/* follow symlinks when writing archive */
int	Pflag;			/* don't follow symlinks when writing archive */
int	Xflag;			/* archive files with same device id only */
int	Yflag;			/* same as Dflg except after name mode */
int	Zflag;			/* same as uflg except after name mode */
int	vfpart;			/* is partial verbose output in progress */
int	patime = 1;		/* preserve file access time */
int	pmtime = 1;		/* preserve file modification times */
int	pmode;			/* preserve file mode bits */
int	pids;			/* preserve file uid/gid */
int	exit_val;		/* exit value */
int	docrc;			/* check/create file crc */
char	*dirptr;		/* destination dir in a copy */
char	*ltmfrmt;		/* -v locale time format (if any) */
sigset_t s_mask;		/* signal mask for cleanup critical sect */

/*
 *	PAX - Portable Archive Interchange
 *
 * 	A utility to read, write, and write lists of the members of archive
 *	files and copy directory hierarchies. A variety of archive formats
 *	are supported (some are described in POSIX 1003.1 10.1):
 *
 *		ustar - 10.1.1 extended tar interchange format
 *		cpio  - 10.1.2 extended cpio interchange format
 *		tar - old BSD 4.3 tar format
 *		binary cpio - old cpio with binary header format
 *		sysVR4 cpio -  with and without CRC
 *
 * This version is a superset of IEEE Std 1003.2b-d3
 *
 * Summary of Extensions to the IEEE Standard:
 *
 * 1	READ ENHANCEMENTS
 * 1.1	Operations which read archives will continue to operate even when
 *	processing archives which may be damaged, truncated, or fail to meet 
 *	format specs in several different ways. Damaged sections of archives
 *	are detected and avoided if possible. Attempts will be made to resync
 *	archive read operations even with badly damaged media.
 * 1.2	Blocksize requirements are not strictly enforced on archive read.
 *	Tapes which have variable sized records can be read without errors.
 * 1.3	The user can specify via the non-standard option flag -E if error
 *	resync operation should stop on a media error, try a specified number
 *	of times to correct, or try to correct forever.
 * 1.4	Sparse files (lseek holes) stored on the archive (but stored with blocks
 *	of all zeros will be restored with holes appropriate for the target
 *	filesystem
 * 1.5	The user is notified whenever something is found during archive
 *	read operations which violates spec (but the read will continue).
 * 1.6	Multiple archive volumes can be read and may span over different
 *	archive devices 
 * 1.7	Rigidly restores all file attributes exactly as they are stored on the
 *	archive.
 * 1.8	Modification change time ranges can be specified via multiple -T
 *	options. These allow a user to select files whose modification time
 *	lies within a specific time range.
 * 1.9	Files can be selected based on owner (user name or uid) via one or more
 *	-U options.
 * 1.10	Files can be selected based on group (group name or gid) via one o
 *	more -G options.
 * 1.11	File modification time can be checked against exisiting file after
 *	name modification (-Z)
 *
 * 2	WRITE ENHANCEMENTS
 * 2.1	Write operation will stop instead of allowing a user to create a flawed
 *	flawed archive (due to any problem).
 * 2.2	Archives writtens by pax are forced to strictly conform to both the
 *	archive and pax the spceific format specifications.
 * 2.3	Blocking size and format is rigidly enforced on writes.
 * 2.4	Formats which may exhibit header overflow problems (they have fields
 *	too small for large file systems, such as inode number storage), use
 *	routines designed to repair this problem. These techniques still
 *	conform to both pax and format specifications, but no longer truncate
 *	these fields. This removes any restrictions on using these archive
 *	formats on large file systems.
 * 2.5	Multiple archive volumes can be written and may span over different
 *	archive devices 
 * 2.6	A archive volume record limit allows the user to specify the number
 *	of bytes stored on an archive volume. When reached the user is
 *	prompted for the next archive volume. This is specified with the
 *	non-standard -B flag. THe limit is rounded up to the next blocksize.
 * 2.7	All archive padding during write use zero filled sections. This makes
 *	it much easier to pull data out of flawed archive during read
 *	operations.
 * 2.8	Access time reset with the -t applies to all file nodes (including
 *	directories).
 * 2.9	Symbolic links can be followed with -L (optional in the spec).
 * 2.10	Modification or inode change time ranges can be specified via
 *	multiple -T options. These allow a user to select files whose
 *	modification or inode change time lies within a specific time range.
 * 2.11	Files can be selected based on owner (user name or uid) via one or more
 *	-U options.
 * 2.12	Files can be selected based on group (group name or gid) via one o
 *	more -G options.
 * 2.13	Symlinks which appear on the command line can be followed (without
 *	following other symlinks; -H flag)
 *
 * 3	COPY ENHANCEMENTS
 * 3.1	Sparse files (lseek holes) can be copied without expanding the holes
 *	into zero filled blocks. The file copy is created with holes which are
 *	appropriate for the target filesystem
 * 3.2	Access time as well as modification time on copied file trees can be
 *	preserved with the appropriate -p options.
 * 3.3	Access time reset with the -t applies to all file nodes (including
 *	directories).
 * 3.4	Symbolic links can be followed with -L (optional in the spec).
 * 3.5	Modification or inode change time ranges can be specified via
 *	multiple -T options. These allow a user to select files whose
 *	modification or inode change time lies within a specific time range.
 * 3.6	Files can be selected based on owner (user name or uid) via one or more
 *	-U options.
 * 3.7	Files can be selected based on group (group name or gid) via one o
 *	more -G options.
 * 3.8	Symlinks which appear on the command line can be followed (without
 *	following other symlinks; -H flag)
 * 3.9  File inode change time can be checked against exisiting file before
 *	name modification (-D)
 * 3.10 File inode change time can be checked against exisiting file after
 *	name modification (-Y)
 * 3.11	File modification time can be checked against exisiting file after
 *	name modification (-Z)
 *
 * 4	GENERAL ENHANCEMENTS
 * 4.1	Internal structure is designed to isolate format dependent and 
 *	independent functions. Formats are selected via a format driver table.
 *	This encourages the addition of new archive formats by only having to
 *	write those routines which id, read and write the archive header.
 */

/*
 * main()
 *	parse options, set up and operate as specified by the user.
 *	any operational flaw will set exit_val to non-zero
 * Return: 0 if ok, 1 otherwise
 */

#if __STDC__
int
main(int argc, char **argv)
#else
int
main(argc, argv)
	int argc;
	char **argv;
#endif
{
	/*
	 * parse options, determine operational mode, general init
	 */
	options(argc, argv);
        if ((gen_init() < 0) || (tty_init() < 0))
		return(exit_val);

	/*
	 * select a primary operation mode 
	 */
	switch(act) {
	case EXTRACT:
		extract();
		break;
	case ARCHIVE:
		archive();
		break;
	case APPND:
		append();
		break;
	case COPY:
		copy();
		break;
	default:
	case LIST:
		list();
		break;
	}
	return(exit_val);
}

/*
 * sig_cleanup()
 *	when interrupted we try to do whatever delayed processing we can.
 *	This is not critical, but we really ought to limit our damage when we
 *	are aborted by the user.
 * Return:
 *	never....
 */

#if __STDC__
void
sig_cleanup(int which_sig)
#else
void
sig_cleanup(which_sig)
	int which_sig;
#endif
{
	/*
	 * restore modes and times for any dirs we may have created
	 * or any dirs we may have read. Set vflag and vfpart so the user
	 * will clearly see the message on a line by itself.
	 */
	vflag = vfpart = 1;
	if (which_sig == SIGXCPU)
		warn(0, "Cpu time limit reached, cleaning up.");
	else
		warn(0, "Signal caught, cleaning up.");

	ar_close();
	proc_dir();
	if (tflag)
		atdir_end();
	exit(1);
}

/*
 * gen_init()
 *	general setup routines. Not all are required, but they really help
 *	when dealing with a medium to large sized archives.
 */

#if __STDC__
static int
gen_init(void)
#else
static int
gen_init()
#endif
{
	struct rlimit reslimit;
	struct sigaction n_hand;
	struct sigaction o_hand;

	/*
	 * Really needed to handle large archives. We can run out of memory for
	 * internal tables really fast when we have a whole lot of files...
	 */
	if (getrlimit(RLIMIT_DATA , &reslimit) == 0){
		reslimit.rlim_cur = reslimit.rlim_max;
		(void)setrlimit(RLIMIT_DATA , &reslimit);
	}

	/*
	 * should file size limits be waived? if the os limits us, this is
	 * needed if we want to write a large archive
	 */
	if (getrlimit(RLIMIT_FSIZE , &reslimit) == 0){
		reslimit.rlim_cur = reslimit.rlim_max;
		(void)setrlimit(RLIMIT_FSIZE , &reslimit);
	}

	/*
	 * increase the size the stack can grow to
	 */
	if (getrlimit(RLIMIT_STACK , &reslimit) == 0){
		reslimit.rlim_cur = reslimit.rlim_max;
		(void)setrlimit(RLIMIT_STACK , &reslimit);
	}

	/*
	 * not really needed, but doesn't hurt
	 */
	if (getrlimit(RLIMIT_RSS , &reslimit) == 0){
		reslimit.rlim_cur = reslimit.rlim_max;
		(void)setrlimit(RLIMIT_RSS , &reslimit);
	}

	/*
	 * Handle posix locale
	 *
	 * set user defines time printing format for -v option 
	 */
	ltmfrmt = getenv("LC_TIME");

	/*
	 * signal handling to reset stored directory times and modes. Since
	 * we deal with broken pipes via failed writes we ignore it. We also
	 * deal with any file size limit thorugh failed writes. Cpu time
	 * limits are caught and a cleanup is forced.
	 */
	if ((sigemptyset(&s_mask) < 0) || (sigaddset(&s_mask, SIGTERM) < 0) ||
	    (sigaddset(&s_mask,SIGINT) < 0)||(sigaddset(&s_mask,SIGHUP) < 0) ||
	    (sigaddset(&s_mask,SIGPIPE) < 0)||(sigaddset(&s_mask,SIGQUIT)<0) ||
	    (sigaddset(&s_mask,SIGXCPU) < 0)||(sigaddset(&s_mask,SIGXFSZ)<0)) {
		warn(1, "Unable to set up signal mask");
		return(-1);
	}
	n_hand.sa_mask = s_mask;
	n_hand.sa_flags = 0;
	n_hand.sa_handler = sig_cleanup;

	if ((sigaction(SIGHUP, &n_hand, &o_hand) < 0) &&
	    (o_hand.sa_handler == SIG_IGN) && 
	    (sigaction(SIGHUP, &o_hand, &o_hand) < 0))
		goto out;

	if ((sigaction(SIGTERM, &n_hand, &o_hand) < 0) &&
	    (o_hand.sa_handler == SIG_IGN) && 
	    (sigaction(SIGTERM, &o_hand, &o_hand) < 0))
		goto out;

	if ((sigaction(SIGINT, &n_hand, &o_hand) < 0) &&
	    (o_hand.sa_handler == SIG_IGN) && 
	    (sigaction(SIGINT, &o_hand, &o_hand) < 0))
		goto out;

	if ((sigaction(SIGQUIT, &n_hand, &o_hand) < 0) &&
	    (o_hand.sa_handler == SIG_IGN) && 
	    (sigaction(SIGQUIT, &o_hand, &o_hand) < 0))
		goto out;

	if ((sigaction(SIGXCPU, &n_hand, &o_hand) < 0) &&
	    (o_hand.sa_handler == SIG_IGN) && 
	    (sigaction(SIGXCPU, &o_hand, &o_hand) < 0))
		goto out;

	n_hand.sa_handler = SIG_IGN;
	if ((sigaction(SIGPIPE, &n_hand, &o_hand) < 0) ||
	    (sigaction(SIGXFSZ, &n_hand, &o_hand) < 0))
		goto out;
	return(0);

    out:
	syswarn(1, errno, "Unable to set up signal handler");
	return(-1);
}

/*
 * usage()
 *	print the usage summary to the user
 */

#if __STDC__
void
usage(void)
#else
void
usage()
#endif
{
	(void)fputs("usage: pax [-cdnv] [-E limit] [-f archive] ", stderr);
	(void)fputs("[-s replstr] ... [-U user] ...", stderr);
	(void)fputs("\n           [-G group] ... ", stderr);
	(void)fputs("[-T [from_date][,to_date]] ... ", stderr);
	(void)fputs("[pattern ...]\n", stderr);
	(void)fputs("       pax -r [-cdiknuvDYZ] [-E limit] ", stderr);
	(void)fputs("[-f archive] [-o options] ... \n", stderr);
	(void)fputs("           [-p string] ... [-s replstr] ... ", stderr);
	(void)fputs("[-U user] ... [-G group] ...\n           ", stderr);
	(void)fputs("[-T [from_date][,to_date]] ... ", stderr);
	(void)fputs(" [pattern ...]\n", stderr);
	(void)fputs("       pax -w [-dituvHLPX] [-b blocksize] ", stderr);
	(void)fputs("[ [-a] [-f archive] ] [-x format] \n", stderr);
	(void)fputs("           [-B bytes] [-s replstr] ... ", stderr);
	(void)fputs("[-o options] ... [-U user] ...", stderr);
	(void)fputs("\n           [-G group] ... ", stderr);
	(void)fputs("[-T [from_date][,to_date][/[c][m]]] ... ", stderr);
	(void)fputs("[file ...]\n", stderr);
	(void)fputs("       pax -r -w [-diklntuvDHLPXYZ]", stderr);
	(void)fputs("[-p string] ... [-s replstr] ... [-U user] ...", stderr);
	(void)fputs("\n           [-G group] ... ", stderr);
	(void)fputs("[-T [from_date][,to_date][/[c][m]]] ... ", stderr);
	(void)fputs("[file ...]\n           directory\n", stderr);
	exit(1);
}
