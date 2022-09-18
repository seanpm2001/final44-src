/* 
 * Mach Operating System
 * Copyright (c) 1994,1993 Carnegie Mellon University
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
 * $Log:	ms_mach_timer_arm.c,v $
 * Revision 2.2  94/12/16  11:45:26  dbg
 * 	RT-MACH merge: renamed 'time_spec' to 'timespec',
 * 	'timer_' to 'mach_timer_'.
 * 	[94/05/23            dbg]
 * 
 * Revision 2.2  93/11/17  18:59:49  dbg
 * 	Created.
 * 	[93/07/15            dbg]
 * 
 */

#include <mach.h>
#include <mach/message.h>

kern_return_t
mach_timer_arm(
	mach_timer_t	timer,
	timespec_t	expire_time,
	timespec_t	interval_time,
	mach_port_t	expire_port,
	thread_t	thread,
	int		flags)
{
	kern_return_t kr;

	kr = syscall_mach_timer_arm(timer, expire_time, interval_time,
					expire_port, thread, flags);
	if (kr == MACH_SEND_INTERRUPTED)
		kr = mig_mach_timer_arm(timer, expire_time, interval_time,
					expire_port, thread, flags);

	return kr;
}
