h10896
s 00001/00001/00271
d D 8.6 94/05/29 00:05:13 mckusick 10 9
c cleanup final nits
e
s 00026/00027/00246
d D 8.5 94/05/26 22:30:52 bostic 9 8
c cleanups and edits for accuracy
e
s 00003/00003/00270
d D 8.4 94/05/21 15:49:30 hibler 8 7
c "osv" should be "osa" for sigaction, note that "code" is sig-specific
e
s 00024/00015/00249
d D 8.3 94/05/20 06:06:54 karels 7 6
c touchups and improvements
e
s 00112/00076/00152
d D 8.2 94/05/16 10:09:37 mckusick 6 5
c massive overhaul to bring up to date for 4.4BSD
e
s 00002/00002/00226
d D 8.1 93/06/08 18:05:20 bostic 5 4
c 4.4BSD snapshot (revision 8.1); add 1993 to copyright
e
s 00004/00003/00224
d D 6.3 91/04/17 10:01:54 bostic 4 3
c new copyright; att/bsd/shared
e
s 00013/00009/00214
d D 6.2 86/05/12 01:00:51 karels 3 2
c update, correct, rephrase and re-propose
e
s 00007/00004/00216
d D 6.1 86/05/08 12:25:27 mckusick 2 1
c update signal interface
e
s 00220/00000/00000
d D 5.1 86/05/08 00:02:42 mckusick 1 0
c document distributed with 4.2BSD
e
u
U
t
T
I 1
D 4
.\" Copyright (c) 1983 Regents of the University of California.
.\" All rights reserved.  The Berkeley software License Agreement
.\" specifies the terms and conditions for redistribution.
E 4
I 4
D 5
.\" Copyright (c) 1983 The Regents of the University of California.
.\" All rights reserved.
E 5
I 5
D 7
.\" Copyright (c) 1983, 1993
E 7
I 7
.\" Copyright (c) 1983, 1993, 1994
E 7
.\"	The Regents of the University of California.  All rights reserved.
E 5
.\"
.\" %sccs.include.redist.roff%
E 4
.\"
.\"	%W% (Berkeley) %G%
.\"
D 2
.\" %M% %I% %E%
E 2
D 6
.sh "Signals
E 6
I 6
.Sh 2 "Signals
E 6
.PP
D 6
.NH 3
Overview
E 6
I 6
.Sh 3 "Overview
E 6
.PP
The system defines a set of \fIsignals\fP that may be delivered
to a process.  Signal delivery resembles the occurrence of a hardware
interrupt: the signal is blocked from further occurrence,
the current process context is saved, and a new one
D 9
is built.  A process may specify
the \fIhandler\fP to which a signal is delivered, or specify that
the signal is to be \fIblocked\fP or \fIignored\fP.  A process may
also specify that a
E 9
I 9
is built.
A process may specify a \fIhandler\fP to which a signal is delivered,
or specify that the signal is to be \fIblocked\fP or \fIignored\fP.
A process may also specify that a
E 9
\fIdefault\fP action is to be taken when signals occur.
.PP
D 9
Some signals
will cause a process to exit when they are not caught.  This
may be accompanied by creation of a \fIcore\fP image file, containing
E 9
I 9
Some signals will cause a process to exit if they are not caught.
This may be accompanied by creation of a \fIcore\fP image file,
containing
E 9
the current memory image of the process for use in post-mortem debugging.
D 9
A process may choose to have signals delivered on a special
stack, so that sophisticated software stack manipulations are possible.
E 9
I 9
A process may also choose to have signals delivered on a special stack,
so that sophisticated software stack manipulations are possible.
E 9
.PP
D 9
All signals have the same \fIpriority\fP.  If multiple signals
D 7
are pending simultaneously, the order in which they are delivered
to a process is implementation specific.  Signal routines execute
E 7
I 7
are pending simultaneously, signals that may be generated by the
E 9
I 9
All signals have the same \fIpriority\fP.
If multiple signals are pending,
signals that may be generated by the
E 9
program's action are delivered first; the order in which other signals
D 9
are delivered to a process is implementation specific.
E 9
I 9
are delivered to a process is not specified.
E 9
Signal routines execute
E 7
with the signal that caused their invocation \fIblocked\fP, but other
D 7
signals may yet occur.  Mechanisms are provided whereby critical sections
E 7
I 7
D 9
signals may yet occur.
E 9
I 9
signals may occur.
E 9
Multiple signals may be delivered on a single entry to the system,
as if signal handlers were interrupted by other signal handlers.
Mechanisms are provided whereby critical sections
E 7
of code may protect themselves against the occurrence of specified signals.
D 6
.NH 3
Signal types
E 6
I 6
.Sh 3 "Signal types
E 6
.PP
The signals defined by the system fall into one of
five classes: hardware conditions,
software conditions, input/output notification, process control, or
resource control.
D 3
The set of signals is defined in the file <signal.h>.
E 3
I 3
D 9
The set of signals is defined in the file \fI<signal.h>\fP.
E 9
I 9
The set of signals is defined by the file \fI<signal.h>\fP.
E 9
E 3
.PP
Hardware signals are derived from exceptional conditions which
may occur during
execution.  Such signals include SIGFPE representing floating
point and other arithmetic exceptions, SIGILL for illegal instruction
D 9
execution, SIGSEGV for addresses outside the currently assigned
area of memory, and SIGBUS for accesses that violate memory
D 7
protection constraints.
E 7
I 7
access constraints.
E 9
I 9
execution, SIGSEGV for attempts to access addresses outside the
currently assigned area of memory,
and SIGBUS for accesses that violate memory access constraints.
E 9
E 7
D 6
Other, more cpu-specific hardware signals exist,
such as those for the various customer-reserved instructions on
the VAX (SIGIOT, SIGEMT, and SIGTRAP). 
E 6
.PP
Software signals reflect interrupts generated by user request:
SIGINT for the normal interrupt signal; SIGQUIT for the more
D 7
powerful \fIquit\fP signal, that normally causes a core image
E 7
I 7
powerful \fIquit\fP signal, which normally causes a core image
E 7
to be generated; SIGHUP and SIGTERM that cause graceful
process termination, either because a user has ``hung up'', or
by user or program request; and SIGKILL, a more powerful termination
signal which a process cannot catch or ignore.
I 2
Programs may define their own asynchronous events using SIGUSR1
and SIGUSR2.
E 2
Other software signals (SIGALRM, SIGVTALRM, SIGPROF)
indicate the expiration of interval timers.
I 6
When a window changes size, a SIGWINCH is sent to the
controlling terminal process group.
E 6
.PP
A process can request notification via a SIGIO signal
when input or output is possible
on a descriptor, or when a \fInon-blocking\fP operation completes.
A process may request to receive a SIGURG signal when an
urgent condition arises. 
.PP
A process may be \fIstopped\fP by a signal sent to it or the members
of its process group.  The SIGSTOP signal is a powerful stop
signal, because it cannot be caught.  Other stop signals
SIGTSTP, SIGTTIN, and SIGTTOU are used when a user request, input
D 3
request, or output request respectively is the reason the process
is being stopped.  A SIGCONT signal is sent to a process when it is
E 3
I 3
request, or output request respectively is the reason for stopping the process.
A SIGCONT signal is sent to a process when it is
E 3
continued from a stopped state.
Processes may receive notification with a SIGCHLD signal when
a child process changes state, either by stopping or by terminating.
.PP
Exceeding resource limits may cause signals to be generated.
D 9
SIGXCPU occurs when a process nears its CPU time limit and SIGXFSZ
warns that the limit on file size creation has been reached.
E 9
I 9
SIGXCPU occurs when a process nears its CPU time limit and
SIGXFSZ when a process reaches the limit on file size.
E 9
D 6
.NH 3
Signal handlers
E 6
I 6
.Sh 3 "Signal handlers
E 6
.PP
D 3
A process has a handler associated with each signal that
controls the way the signal is delivered.
E 3
I 3
A process has a handler associated with each signal.
The handler controls the way the signal is delivered.
E 3
D 6
The call
E 6
I 6
The call:
E 6
.DS
D 6
#include <signal.h>

._f
struct sigvec {
	int	(*sv_handler)();
	int	sv_mask;
D 2
	int	sv_onstack;
E 2
I 2
	int	sv_flags;
E 6
I 6
.TS
l s s
l l l.
struct sigaction {
	void	(*sa_handler)();
	sigset_t	sa_mask;
	int	sa_flags;
E 6
E 2
};
D 6

sigvec(signo, sv, osv)
int signo; struct sigvec *sv; result struct sigvec *osv;
E 6
I 6
.TE
E 6
.DE
D 6
assigns interrupt handler address \fIsv_handler\fP to signal \fIsigno\fP.
E 6
I 6
.DS
.Fd sigaction 3 "setup software signal handler
D 9
sigaction(signo, sa, osa)
E 9
I 9
sigaction(signo, sa, osa);
E 9
int signo; struct sigaction *sa; result struct sigaction *osa;
.DE
assigns interrupt handler address \fIsa_handler\fP to signal \fIsigno\fP.
E 6
Each handler address
specifies either an interrupt routine for the signal, that the
signal is to be ignored,
or that a default action (usually process termination) is to occur
if the signal occurs.
The constants
D 6
SIG_IGN and SIG_DEF used as values for \fIsv_handler\fP
E 6
I 6
D 7
SIG_IGN and SIG_DEF used as values for \fIsa_handler\fP
E 6
cause ignoring or defaulting of a condition.
E 7
I 7
SIG_IGN and SIG_DFL used as values for \fIsa_handler\fP
cause ignoring or defaulting of a condition, respectively.
E 7
D 2
The \fIsv_mask\fP and \fIsv_onstack\fP values specify the
signal mask to be used when the handler is invoked and
E 2
I 2
D 6
The \fIsv_mask\fP value specifies the
E 6
I 6
The \fIsa_mask\fP value specifies the
E 6
D 3
signal mask to be used when the handler is invoked.
E 3
I 3
signal mask to be used when the handler is invoked; it implicitly includes
the signal which invoked the handler.
D 6
Signal masks include one bit for each signal;
the mask for a signal \fIsigno\fP is provided by the macro
\fIsigmask\fP(\fIsigno\fP), from \fI<signal.h>\fP.
E 3
\fISv_flags\fP specifies whether system calls should be
restarted if the signal handler returns and
E 6
I 6
Signal masks include one bit for each signal.
The following macros, defined in \fIsignal.h\fP,
create an empty mask, then put \fIsigno\fP into it:
.DS
sigemptyset(set);
sigaddset(set, signo);
result sigset_t *set; int signo;
.DE
D 7
\fISa_flags\fP specifies whether system calls should be
E 7
I 7
\fISa_flags\fP specifies whether pending system calls should be
E 7
restarted if the signal handler returns (SA_RESTART) and
E 6
E 2
whether the handler should operate on the normal run-time
D 7
stack or a special signal stack (see below).  If \fIosv\fP
E 7
I 7
D 8
stack or a special signal stack (SA_ONSTACK; see below).  If \fIosv\fP
E 7
is non-zero, the previous signal vector is returned.
E 8
I 8
stack or a special signal stack (SA_ONSTACK; see below).
If \fIosa\fP is non-zero, the previous signal handler information is returned.
E 8
.PP
When a signal condition arises for a process, the signal
is added to a set of signals pending for the process.
If the signal is not currently \fIblocked\fP by the process
D 9
then it will be delivered.  The process of signal delivery
adds the signal to be delivered and those signals
specified in the associated signal
E 9
I 9
it then will be delivered.
The process of signal delivery adds the signal to be delivered
and those signals specified in the associated signal
E 9
D 6
handler's \fIsv_mask\fP to a set of those \fImasked\fP
E 6
I 6
handler's \fIsa_mask\fP to a set of those \fImasked\fP
E 6
for the process, saves the current process context,
and places the process in the context of the signal
handling routine.  The call is arranged so that if the signal
D 7
handling routine exits normally the signal mask will be restored
E 7
I 7
handling routine returns normally, the signal mask will be restored
E 7
and the process will resume execution in the original context.
D 9
If the process wishes to resume in a different context, then
it must arrange to restore the signal mask itself.
E 9
.PP
The mask of \fIblocked\fP signals is independent of handlers for
D 3
signals.  It prevents signals from being delivered much as a
raised hardware interrupt priority level prevents hardware interrupts.
E 3
I 3
signals.  It delays signals from being delivered much as a
raised hardware interrupt priority level delays hardware interrupts.
E 3
Preventing an interrupt from occurring by changing the handler is analogous to
disabling a device from further interrupts.
D 6
.PP
The signal handling routine \fIsv_handler\fP is called by a C call
of the form
E 6
I 6
.LP
The signal handling routine \fIsa_handler\fP is called by a C call
of the form:
E 6
.DS
D 6
(*sv_handler)(signo, code, scp);
E 6
I 6
(*sa_handler)(signo, code, scp);
E 6
int signo; long code; struct sigcontext *scp;
.DE
The \fIsigno\fP gives the number of the signal that occurred, and
D 8
the \fIcode\fP, a word of information supplied by the hardware.
E 8
I 8
the \fIcode\fP, a word of signal-specific information supplied by the hardware.
E 8
The \fIscp\fP parameter is a pointer to a machine-dependent
structure containing the information for restoring the
context before the signal.
D 6
.NH 3
Sending signals
.PP
A process can send a signal to another process or group of processes
with the calls:
E 6
I 6
D 7
Normally this context will be restored when the process returns or
.Fn longjmp 's
from the signal handler.
E 7
I 7
Normally this context will be restored when the signal handler returns.
E 7
However, a process may do so at any time by using the call:
E 6
.DS
I 6
.Fd sigreturn 1 "return from a signal
sigreturn(scp);
struct sigcontext *scp;
.DE
I 7
If the signal handler makes a call to
.Fn longjmp ,
the signal mask at the time of the corresponding
.Fn setjmp
is restored.
E 7
.Sh 3 "Sending signals
.LP
A process can send a signal to another process or processes group
with the call:
.DS
.Fd kill 2 "send signal to a process
E 6
kill(pid, signo)
D 9
int pid, signo;
E 9
I 9
pid_t pid; int signo;
E 9
D 6

killpgrp(pgrp, signo)
E 6
I 6
.DE
For compatibility with old systems,
a compatibility routine is provided to send a signal to a process group:
.DS
.Fd killpg 2 "send signal to a process group
killpg(pgrp, signo)
E 6
D 9
int pgrp, signo;
E 9
I 9
pid_t pgrp; int signo;
E 9
.DE
Unless the process sending the signal is privileged,
D 3
it and the process receiving the signal must have the same effective user id.
E 3
I 3
it must have the same effective user id as the process receiving the signal.
E 3
.PP
D 10
Signals are also sent implicitly from a terminal device to the
E 10
I 10
Signals also are sent implicitly from a terminal device to the
E 10
process group associated with the terminal when certain input characters
are typed.
D 6
.NH 3
Protecting critical sections
.PP
To block a section of code against one or more signals, a \fIsigblock\fP
call may be used to add a set of signals to the existing mask, returning
the old mask:
E 6
I 6
.Sh 3 "Protecting critical sections
.LP
The
.Fn sigprocmask
system call is used to manipulate the mask of blocked signals:
E 6
.DS
D 6
oldmask = sigblock(mask);
result long oldmask; long mask;
E 6
I 6
.Fd sigprocmask 3 "manipulate current signal mask
sigprocmask(how, newmask, oldmask);
int how; sigset_t *newmask; result sigset_t *oldmask;
E 6
.DE
D 6
The old mask can then be restored later with \fIsigsetmask\fP\|,
.DS
oldmask = sigsetmask(mask);
result long oldmask; long mask;
.DE
The \fIsigblock\fP call can be used to read the current mask
by specifying an empty \fImask\fP\|.
E 6
I 6
The actions done by
.Fn sigprocmask
are to add to the list of masked signals (SIG_BLOCK),
delete from the list of masked signals (SIG_UNBLOCK),
and block a specific set of signals (SIG_SETMASK).
The
.Fn sigprocmask
call can be used to read the current mask
by specifying SIG_BLOCK with an empty \fInewmask\fP\|.
E 6
.PP
It is possible to check conditions with some signals blocked,
and then to pause waiting for a signal and restoring the mask, by using:
.DS
D 6
sigpause(mask);
long mask;
E 6
I 6
.Fd sigsuspend 1 "atomically release blocked signals and wait for interrupt
sigsuspend(mask);
sigset_t *mask;
E 6
.DE
D 6
.NH 3
Signal stacks
.PP
E 6
I 6
It is also possible to find out which blocked signals
are pending delivery using the call:
.DS
.Fd sigpending 1 "get pending signals
sigpending(mask);
result sigset_t *mask;
.DE
.Sh 3 "Signal stacks
.LP
E 6
Applications that maintain complex or fixed size stacks can use
D 6
the call
E 6
I 6
the call:
E 6
.DS
D 6
._f
struct sigstack {
E 6
I 6
.TS
l s s
l l l.
struct sigaltstack {
E 6
	caddr_t	ss_sp;
D 6
	int	ss_onstack;
E 6
I 6
	long	ss_size;
	int	ss_flags;
E 6
};
D 6

sigstack(ss, oss)
struct sigstack *ss; result struct sigstack *oss;
E 6
I 6
.TE
E 6
.DE
D 6
to provide the system with a stack based at \fIss_sp\fP for delivery
of signals. The value \fIss_onstack\fP indicates whether the
E 6
I 6
.DS
.Fd sigaltstack 2 "set and/or get signal stack context
sigaltstack(ss, oss)
struct sigaltstack *ss; result struct sigaltstack *oss;
.DE
to provide the system with a stack based at \fIss_sp\fP of size
\fIss_size\fP for delivery of signals.
The value \fIss_flags\fP indicates whether the
E 6
process is currently on the signal stack,
a notion maintained in software by the system.
.PP
D 7
When a signal is to be delivered, the system checks whether
E 7
I 7
When a signal is to be delivered to a handler for which the SA_ONSTACK
flag was set, the system checks whether
E 7
the process is on a signal stack.  If not, then the process is switched
D 6
to the signal stack for delivery, with the return from the signal
arranged to restore the previous stack.
.PP
If the process wishes to take a non-local exit from the signal routine,
or run code from the signal stack that uses a different stack,
a \fIsigstack\fP call should be used to reset the signal stack.
E 6
I 6
to the signal stack for delivery,
with the return from the signal doing a
.Fn sigreturn
to restore the previous stack.
If the process takes a non-local exit from the signal routine,
.Fn longjmp
will do a
.Fn sigreturn
call to switch back to the run-time stack.
E 6
E 1
