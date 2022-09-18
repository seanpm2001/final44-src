h25328
s 00008/00008/00163
d D 8.2 95/04/03 09:00:14 mckusick 5 4
c sleaze new sysent format by using narg * 4 for argsize
e
s 00000/00000/00171
d D 8.1 93/06/11 15:18:52 bostic 4 3
c 4.4BSD snapshot (revision 8.1)
e
s 00005/00005/00166
d D 7.3 93/05/27 21:45:15 torek 3 2
c fully bracket those initializers (for gcc2 -W)
e
s 00002/00002/00169
d D 7.2 92/10/11 12:58:16 bostic 2 1
c make kernel includes standard
e
s 00171/00000/00000
d D 7.1 92/07/13 00:40:45 torek 1 0
c date and time created 92/07/13 00:40:45 by torek
e
u
U
t
T
I 1
#! /bin/sh -
#	%W% (Berkeley) %G%

set -e

# name of compat option:
compat=XXX_UNUSED

# output files:
sysnames="sun_syscalls.c"
syshdr="sun_syscall.h"
syssw="sun_sysent.c"

# tmp files:
sysdcl="sysent.dcl"
syscompat="sysent.compat"
sysent="sysent.switch"

trap "rm $sysdcl $syscompat $sysent" 0

case $# in
    0)	echo "Usage: $0 input-file" 1>&2
	exit 1
	;;
esac

awk < $1 "
	BEGIN {
		sysdcl = \"$sysdcl\"
		syscompat = \"$syscompat\"
		sysent = \"$sysent\"
		sysnames = \"$sysnames\"
		syshdr = \"$syshdr\"
		compat = \"$compat\"
		infile = \"$1\"
		"'

		printf "/*\n * System call switch table.\n *\n" > sysdcl
		printf " * DO NOT EDIT-- this file is automatically generated.\n" > sysdcl

		printf "\n#ifdef %s\n", compat > syscompat
D 5
		printf "#define compat(n, name) n, __CONCAT(o,name)\n\n" > syscompat
E 5
I 5
		printf "#define compat(n, name) n, n * 4, __CONCAT(compat_43_,name)\n\n" > syscompat
E 5

		printf "/*\n * System call names.\n *\n" > sysnames
		printf " * DO NOT EDIT-- this file is automatically generated.\n" > sysnames

		printf "/*\n * System call numbers.\n *\n" > syshdr
		printf " * DO NOT EDIT-- this file is automatically generated.\n" > syshdr
	}
	NR == 1 {
		printf " * created from%s\n */\n\n", $0 > sysdcl
D 2
		printf "#include \"param.h\"\n" > sysdcl
		printf "#include \"systm.h\"\n\n" > sysdcl
E 2
I 2
		printf "#include <sys/param.h>\n" > sysdcl
		printf "#include <sys/systm.h>\n\n" > sysdcl
E 2
		printf "int\tnosys();\n\n" > sysdcl

		printf "struct sysent sunsys[] = {\n" > sysent

		printf " * created from%s\n */\n\n", $0 > sysnames
		printf "char *syscallnames[] = {\n" > sysnames

		printf " * created from%s\n */\n\n", $0 > syshdr
		next
	}
	NF == 0 || $1 ~ /^;/ {
		next
	}
	$1 ~ /^#[ 	]*if/ {
		print > sysent
		print > sysdcl
		print > syscompat
		print > sysnames
		savesyscall = syscall
		next
	}
	$1 ~ /^#[ 	]*else/ {
		print > sysent
		print > sysdcl
		print > syscompat
		print > sysnames
		syscall = savesyscall
		next
	}
	$1 ~ /^#/ {
		print > sysent
		print > sysdcl
		print > syscompat
		print > sysnames
		next
	}
	syscall != $1 {
		printf "%s: line %d: syscall number out of sync at %d\n", \
		   infile, NR, syscall
		printf "line is:\n"
		print
		exit 1
	}
	{	comment = $4
		for (i = 5; i <= NF; i++)
			comment = comment " " $i
		if (NF < 5)
			$5 = $4
	}
	$2 == "STD" {
		printf("int\t%s();\n", $4) > sysdcl
D 3
		printf("\t%d, %s,\t\t\t/* %d = %s */\n", \
E 3
I 3
D 5
		printf("\t{ %d, %s },\t\t\t/* %d = %s */\n", \
E 3
		    $3, $4, syscall, $5) > sysent
E 5
I 5
		printf("\t{ %d, %d, %s },\t\t\t/* %d = %s */\n", \
		    $3, $3 * 4, $4, syscall, $5) > sysent
E 5
		printf("\t\"%s\",\t\t\t/* %d = %s */\n", \
		    $5, syscall, $5) > sysnames
		printf("#define\tSYS_%s\t%d\n", \
		    $5, syscall) > syshdr
		syscall++
		next
	}
	$2 == "COMPAT" {
D 5
		printf("int\to%s();\n", $4) > syscompat
E 5
I 5
		printf("int\tcompat_43_%s();\n", $4) > syscompat
E 5
D 3
		printf("\tcompat(%d,%s),\t\t/* %d = old %s */\n", \
E 3
I 3
		printf("\t{ compat(%d,%s) },\t\t/* %d = old %s */\n", \
E 3
		    $3, $4, syscall, $5) > sysent
		printf("\t\"old.%s\",\t\t/* %d = old %s */\n", \
		    $5, syscall, $5) > sysnames
		printf("\t\t\t\t/* %d is old %s */\n", \
		    syscall, comment) > syshdr
		syscall++
		next
	}
	$2 == "LIBCOMPAT" {
D 5
		printf("int\to%s();\n", $4) > syscompat
E 5
I 5
		printf("int\tcompat_43_%s();\n", $4) > syscompat
E 5
D 3
		printf("\tcompat(%d,%s),\t\t/* %d = old %s */\n", \
E 3
I 3
		printf("\t{ compat(%d,%s) },\t\t/* %d = old %s */\n", \
E 3
		    $3, $4, syscall, $5) > sysent
		printf("\t\"old.%s\",\t\t/* %d = old %s */\n", \
		    $5, syscall, $5) > sysnames
		printf("#define\tSYS_%s\t%d\t/* compatibility; still used by libc */\n", \
		    $5, syscall) > syshdr
		syscall++
		next
	}
	$2 == "OBSOL" {
D 3
		printf("\t0, nosys,\t\t\t/* %d = obsolete %s */\n", \
E 3
I 3
D 5
		printf("\t{ 0, nosys },\t\t\t/* %d = obsolete %s */\n", \
E 5
I 5
		printf("\t{ 0, 0, nosys },\t\t\t/* %d = obsolete %s */\n", \
E 5
E 3
		    syscall, comment) > sysent
		printf("\t\"obs_%s\",\t\t\t/* %d = obsolete %s */\n", \
		    $4, syscall, comment) > sysnames
		printf("\t\t\t\t/* %d is obsolete %s */\n", \
		    syscall, comment) > syshdr
		syscall++
		next
	}
	$2 == "UNIMPL" {
D 3
		printf("\t0, nosys,\t\t\t/* %d = %s */\n", \
E 3
I 3
D 5
		printf("\t{ 0, nosys },\t\t\t/* %d = %s */\n", \
E 5
I 5
		printf("\t{ 0, 0, nosys },\t\t\t/* %d = %s */\n", \
E 5
E 3
		    syscall, comment) > sysent
		printf("\t\"#%d\",\t\t\t/* %d = %s */\n", \
		    syscall, syscall, comment) > sysnames
		syscall++
		next
	}
	{
		printf "%s: line %d: unrecognized keyword %s\n", infile, NR, $2
		exit 1
	}
	END {
		printf("\n#else /* %s */\n", compat) > syscompat
D 5
		printf("#define compat(n, name) 0, nosys\n") > syscompat
E 5
I 5
		printf("#define compat(n, name) 0, 0, nosys\n") > syscompat
E 5
		printf("#endif /* %s */\n\n", compat) > syscompat

		printf("};\n\n") > sysent
		printf("int\tnsunsys = sizeof(sunsys) / sizeof(sunsys[0]);\n") > sysent

		printf("};\n") > sysnames
	} '

cat $sysdcl $syscompat $sysent >$syssw

chmod 444 $sysnames $syshdr $syssw
E 1
