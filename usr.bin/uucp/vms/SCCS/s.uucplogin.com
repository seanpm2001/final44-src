h46537
s 00000/00000/00017
d D 8.1 93/06/06 22:16:21 bostic 2 1
c 4.4BSD snapshot (revision 8.1)
e
s 00017/00000/00000
d D 5.1 91/04/24 17:50:35 bostic 1 0
c date and time created 91/04/24 17:50:35 by bostic
e
u
U
t
T
I 1
$! UUCP Login File
$ set nocontrol
$ if "''F$Mode()'" .NES. "INTERACTIVE" THEN $ GOTO BATCH
$ nettest:='f$logical("SYS$NET")
$ if nettest .nes. "" then goto batch		! Network task
$ deass sys$input
$ uucico := $e$disk:[eunice.usr.lib.uucp]uucico uucico
$! drain:= $e$disk:[eunice.usr.lib.uucp]drain drain
$ uucico
$! drain
$ assi nl: sys$output
$ assi nl: sys$error
$ logout/brief
$ stop/id=0
$ !
$ BATCH:
$ exit
E 1
