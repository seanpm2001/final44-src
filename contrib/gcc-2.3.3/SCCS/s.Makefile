h35074
s 00023/00030/01839
d D 1.2 93/06/04 17:19:17 elan 2 1
c Unified Makefile for all targets!!!
e
s 01869/00000/00000
d D 1.1 93/06/01 17:49:11 elan 1 0
c date and time created 93/06/01 17:49:11 by elan
e
u
U
t
T
I 1
# Makefile for GNU C compiler.
#   Copyright (C) 1987, 1988, 1990, 1991, 1992 Free Software Foundation, Inc.

#This file is part of GNU CC.

#GNU CC is free software; you can redistribute it and/or modify
#it under the terms of the GNU General Public License as published by
#the Free Software Foundation; either version 2, or (at your option)
#any later version.

#GNU CC is distributed in the hope that it will be useful,
#but WITHOUT ANY WARRANTY; without even the implied warranty of
#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#GNU General Public License for more details.

#You should have received a copy of the GNU General Public License
#along with GNU CC; see the file COPYING.  If not, write to
#the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.

# The targets for external use include:
# all, doc, proto, install, install-cross, install-cross-rest,
# uninstall, TAGS, mostlyclean, clean, distclean, realclean,
# stage1, stage2, stage3, stage4.

# Suppress smart makes who think they know how to automake Yacc files
.y.c:

# Variables that exist for you to override.
# See below for how to change them for certain systems.

# Selection of languages to be made.
D 2
LANGUAGES = c c++ objective-c proto
E 2
I 2
LANGUAGES = c c++
E 2

ALLOCA =
ALLOCA_FLAGS =
ALLOCA_FINISH = true

# Various ways of specifying flags for compilations:  
# CFLAGS is for the user to override to, e.g., do a bootstrap with -O2.
# BOOT_CFLAGS is the value of CFLAGS to pass
# to the stage2 and stage3 compilations
# XCFLAGS is used for most compilations but not when using the GCC just built.
XCFLAGS =
CFLAGS = -g
BOOT_CFLAGS = -O $(CFLAGS)
# These exists to be overridden by the x-* and t-* files, respectively.
X_CFLAGS =
T_CFLAGS =

X_CPPFLAGS =
T_CPPFLAGS =

CC = cc
BISON = bison
BISONFLAGS =
AR = ar
OLDAR_FLAGS = qc
AR_FLAGS = rc
SHELL = /bin/sh
# on sysV, define this as cp.
INSTALL = install -c
# These permit overriding just for certain files.
INSTALL_PROGRAM = $(INSTALL) -m 775
INSTALL_DATA = $(INSTALL) -m 664
SYMLINK = ln -s

# Define this as & to perform parallel make on a Sequent.
# Note that this has some bugs, and it seems currently necessary 
# to compile all the gen* files first by hand to avoid erroneous results.
P =

# How to invoke ranlib.
RANLIB = ranlib
# Test to use to see whether ranlib exists on the system.
RANLIB_TEST = [ -f /usr/bin/ranlib -o -f /bin/ranlib ]

# Compiler to use for compiling libgcc1.a.
# OLDCC should not be the GNU C compiler,
# since that would compile typical libgcc1.a functions such as mulsi3
# into infinite recursions.
OLDCC = cc

# CFLAGS for use with OLDCC, for compiling libgcc1.a.
# NOTE: -O does not work on some Unix systems!
CCLIBFLAGS = -O

# Version of ar to use when compiling libgcc1.a.
OLDAR = ar

# The GCC to use for compiling libgcc2.a, enquire, and cross-test.
# Usually the one we just built.
# Don't use this as a dependency--use $(GCC_PASSES) or $(GCC_PARTS).
GCC_FOR_TARGET = ./xgcc -B./

# This is used instead of ALL_CFLAGS when compiling with GCC_FOR_TARGET.
# It omits XCFLAGS, and specifies -B./.
# It also specifies -B$(tooldir)/ to find as and ld for a cross compiler.
GCC_CFLAGS=$(INTERNAL_CFLAGS) $(X_CFLAGS) $(T_CFLAGS) $(CFLAGS) $(CROSS_GCC_CFLAGS)

# Special flags for compiling enquire.
# We disable optimization to make floating point more reliable.
ENQUIRE_CFLAGS = -DNO_MEM -DNO_STDDEF -DNO_LONG_DOUBLE_IO -O0
ENQUIRE_LDFLAGS = $(LDFLAGS)

# Tools to use when building a cross-compiler.
# These are used because `configure' appends `cross-make'
# to the makefile when making a cross-compiler.

TARGET_TOOLPREFIX = $(tooldir)/bin/
AR_FOR_TARGET = $(TARGET_TOOLPREFIX)ar
AR_FOR_TARGET_FLAGS = rc
RANLIB_FOR_TARGET = $(TARGET_TOOLPREFIX)ranlib
RANLIB_TEST_FOR_TARGET = [ -f $(TARGET_TOOLPREFIX)ranlib ]
CROSS_TOOLS =

# Dir to search for system headers.  Overridden by cross-make.
SYSTEM_HEADER_DIR = /usr/include

# There may be a premade insn-attrtab.c for this machine.
# (You could rebuild it with genattrtab as usual, but it takes a long time.)
# PREMADE_ATTRTAB is the file name of the file to use.
# PREMADE_ATTRTAB_MD is the md file it corresponds to.
PREMADE_ATTRTAB_MD = Makefile  # Guaranteed not to cmp equal to md.
PREMADE_ATTRTAB = 

D 2
target=m68k-hp-bsd
xmake_file=../Makefile.in
tmake_file=../Makefile.in
E 2
I 2

.if ${MACHINE} == "hp300"
target = m68k-hp-bsd
.elif ${MACHINE} == "luna68k"
target =  m68k-omron-bsd
.elif ${MACHINE} == "mips"
target = mips-dec-bsd
.elif ${MACHINE} == "sparc"
target = sparc
.endif


#xmake_file=../Makefile.in
#tmake_file=../Makefile.in

E 2
version=`sed -e 's/.*\"\([^ \"]*\)[ \"].*/\1/' < $(srcdir)/version.c`

# Directory where sources are, from where we are.
srcdir = .
# Common prefix for installation directories.
# NOTE: This directory must exist when you start installation.
prefix = /usr/libexec
# Directory in which to put host dependent programs and libraries
exec_prefix = $(prefix)/gcc2
# Directory in which to put the executable for the command `gcc'
bindir = $(exec_prefix)
# Directory in which to put the directories used by the compiler.
libdir = $(exec_prefix)
# Directory in which the compiler finds executables, libraries, etc.
libsubdir = $(exec_prefix)
# Directory for header file assert.h.
#includedir = 
# assertdir is overridden in cross-make.
assertdir = $(includedir)
# Extension (if any) to put in installed man-page filename.
manext = .0
# Directory in which to put man pages.
mandir = /usr/contrib/man/cat1
# Directory in which to find other cross-compilation tools and headers.
# Used in install-cross.
tooldir = $(prefix)/$(target)
# Dir for temp files.
tmpdir = /tmp

GROFF=nroff -man

# Additional system libraries to link with.
CLIB=

# Change this to a null string if obstacks are installed in the
# system library.
OBSTACK= obstack.o

# Specify the rule for actually making libgcc1.a.
# The value may be empty; that means to do absolutely nothing
# with or for libgcc1.a.
D 2
LIBGCC1 = libgcc1.a
E 2
I 2
#LIBGCC1 = libgcc1.a
LIBGCC1 =
E 2

# Specify the rule for actually making libgcc2.a.
LIBGCC2 = libgcc2.a

# Options to use when compiling libgcc2.a.
# -g1 causes output of debug info only for file-scope entities.
# we use this here because that should be enough, and also
# so that -g1 will be tested.
LIBGCC2_CFLAGS = -O2 $(GCC_CFLAGS) -g1

# List of extra executables that should be compiled for this target machine
# that are used for compiling from source code to object code.
# The rules for compiling them should be in the t-* file for the machine.
EXTRA_PASSES =

# Like EXTRA_PASSES, but these are used when linking.
EXTRA_PROGRAMS = 

# List of extra object files that should be compiled for this target machine.
# The rules for compiling them should be in the t-* file for the machine.
EXTRA_PARTS =

# List of extra object files that should be compiled and linked with
# compiler proper (cc1, cc1obj, cc1plus).
EXTRA_OBJS =

# List of additional header files to install.
# Often this is edited directly by `configure'.
#EXTRA_HEADERS = math-68881.h
EXTRA_HEADERS =

# Set this to `ld' to enable use of collect2.
# USE_COLLECT2 =
# It is convenient for configure to add the assignment at the beginning,
# so don't override it here.

# List of extra C and assembler files to add to libgcc1.a.
# Assembler files should have names ending in `.asm'.
LIB1FUNCS_EXTRA = 

# List of extra C and assembler files to add to libgcc2.a.
# Assembler files should have names ending in `.asm'.
LIB2FUNCS_EXTRA = 

# Program to convert libraries.
LIBCONVERT = 

# Control whether header files are installed.
# We will change this to install-fixincludes eventually.
# It would be useful for people to try this now.
#INSTALL_HEADERS=install-headers


# Select which version of fixincludes to use (I.E. regular versus SVR4)
FIXINCLUDES = fixincludes

# Additional directories of header files to run fixincludes on.
# On most systems, this is empty.
OTHER_FIXINCLUDES_DIRS=

# List of things which should already be built whenever we try to use xgcc
# to compile anything (without linking).
GCC_PASSES=xgcc cc1 cpp $(EXTRA_PASSES)

# List of things which should already be built whenever we try to use xgcc
# to link anything.
GCC_PARTS=$(GCC_PASSES) libgcc.a $(EXTRA_PROGRAMS) $(USE_COLLECT2) $(EXTRA_PARTS)

# Directory to link to, when using the target `maketest'.
DIR = ../gcc

# Flags to use when cross-building GCC.
# Prefix to apply to names of object files when using them
# to run on the machine we are compiling on.
HOST_PREFIX=
# Prefix to apply to names of object files when compiling them
# to run on the machine we are compiling on.
# The default for this variable is chosen to keep these rules 
# out of the way of the other rules for compiling the same source files.
HOST_PREFIX_1=loser-
HOST_CC=$(CC)
HOST_CFLAGS=$(ALL_CFLAGS)
HOST_CLIB=$(CLIB)
HOST_LDFLAGS=$(LDFLAGS)
HOST_CPPFLAGS=$(ALL_CPPFLAGS)
HOST_ALLOCA=$(ALLOCA)
HOST_MALLOC=$(MALLOC)
HOST_OBSTACK=$(OBSTACK)

# Choose the real default target.
ALL=all.internal

# Choose the real install target.
INSTALL_TARGET=install-native

# End of variables for you to override.

# Definition of `all' is here so that new rules inserted by sed
# do not specify the default target.
# The real definition is under `all.internal' (for native compilers)
# or `all.cross' (for cross compilers).
D 2
all: all.indirect
E 2
I 2
all: banner all.indirect
E 2

I 2
banner:
	@echo Creating gcc for target = $(target)...
	

E 2
# This tells GNU Make version 3 not to put all variables in the environment.
.NOEXPORT:

# sed inserts variable overrides after the following line.
####target overrides
####host overrides
####cross overrides
####build overrides

# Now figure out from those variables how to compile and link.

all.indirect: $(ALL)

# IN_GCC tells obstack.h to use gstddef.h.
INTERNAL_CFLAGS = $(CROSS) -DIN_GCC

# This is the variable actually used when we compile.
ALL_CFLAGS = $(INTERNAL_CFLAGS) $(X_CFLAGS) $(T_CFLAGS) $(CFLAGS) $(XCFLAGS)

# Likewise.
ALL_CPPFLAGS = $(CPPFLAGS) $(X_CPPFLAGS) $(T_CPPFLAGS)

# Even if ALLOCA is set, don't use it if compiling with GCC.
USE_ALLOCA= ` case "${CC}" in "${OLDCC}") echo ${ALLOCA} ;; esac `
USE_HOST_ALLOCA= ` case "${HOST_CC}"@"${HOST_ALLOCA}" in "${OLDCC}"@?*) echo ${HOST_PREFIX}${HOST_ALLOCA} ;; esac `
USE_HOST_MALLOC= ` case "${HOST_MALLOC}" in ?*) echo ${HOST_PREFIX}${HOST_MALLOC} ;; esac `
USE_HOST_OBSTACK= ` case "${HOST_OBSTACK}" in ?*) echo ${HOST_PREFIX}${HOST_OBSTACK} ;; esac `

# Dependency on obstack, alloca, malloc or whatever library facilities
# are not installed in the system libraries.
# We don't use USE_ALLOCA because backquote expansion doesn't work in deps.
LIBDEPS= $(OBSTACK) $(ALLOCA) $(MALLOC)

# Likewise, for use in the tools that must run on this machine
# even if we are cross-building GCC.
# We don't use USE_ALLOCA because backquote expansion doesn't work in deps.
HOST_LIBDEPS= $(HOST_PREFIX)$(HOST_OBSTACK) $(HOST_PREFIX)$(HOST_ALLOCA) $(HOST_PREFIX)$(HOST_MALLOC)

# How to link with both our special library facilities
# and the system's installed libraries.
LIBS = $(OBSTACK) $(USE_ALLOCA) $(MALLOC) $(CLIB)

# Likewise, for use in the tools that must run on this machine
# even if we are cross-building GCC.
HOST_LIBS = $(USE_HOST_OBSTACK) $(USE_HOST_ALLOCA) $(USE_HOST_MALLOC)  \
	    $(HOST_CLIB)

HOST_RTL = $(HOST_PREFIX)rtl.o
HOST_RTLANAL = $(HOST_PREFIX)rtlanal.o
HOST_PRINT = $(HOST_PREFIX)print-rtl.o

# Specify the directories to be searched for header files.
# Both . and srcdir are used, in that order,
# so that tm.h and config.h will be found in the compilation
# subdirectory rather than in the source directory.
INCLUDES = -I. -I$(srcdir) -I$(srcdir)/config
SUBDIR_INCLUDES = -I.. -I../$(srcdir) -I../$(srcdir)/config

# Always use -I$(srcdir)/config when compiling.
.c.o:
	$(CC) -c $(ALL_CFLAGS) $(ALL_CPPFLAGS) $(INCLUDES) $<

# This tells GNU make version 3 not to export all the variables
# defined in this file into the environment.
.NOEXPORT:

# Lists of files for various purposes.

# A list of all the language-specific executables.
COMPILERS = cc1 cc1plus cc1obj

# Language-specific object files for C.
C_OBJS = c-parse.o c-lang.o c-lex.o \
   c-decl.o c-typeck.o c-convert.o c-aux-info.o c-common.o c-pragma.o

# Language-specific object files for Objectionable C.
OBJC_OBJS = objc-parse.o objc-actions.o \
   c-lex.o c-decl.o c-typeck.o c-convert.o c-aux-info.o c-common.o c-pragma.o

# Language-specific object files for C++.
CPLUS_OBJS = cp-parse.o cp-decl.o cp-decl2.o \
   cp-typeck.o cp-type2.o cp-tree.o cp-ptree.o \
   cp-cvt.o cp-search.o cp-lex.o cp-gc.o cp-call.o \
   cp-class.o cp-init.o cp-method.o cp-except.o \
   cp-expr.o cp-pt.o cp-edsel.o cp-xref.o \
   $(CPLUS_INPUT) cp-spew.o c-common.o

# Language-independent object files.
OBJS = toplev.o version.o tree.o print-tree.o stor-layout.o fold-const.o \
 function.o stmt.o expr.o calls.o expmed.o explow.o optabs.o varasm.o \
 rtl.o print-rtl.o rtlanal.o emit-rtl.o \
 dbxout.o sdbout.o dwarfout.o xcoffout.o \
 integrate.o jump.o cse.o loop.o unroll.o flow.o stupid.o combine.o \
 regclass.o local-alloc.o global.o reload.o reload1.o caller-save.o \
 insn-peep.o reorg.o sched.o final.o recog.o reg-stack.o \
 insn-recog.o insn-extract.o insn-output.o insn-emit.o \
 insn-attrtab.o aux-output.o getpwd.o $(EXTRA_OBJS)

# GEN files are listed separately, so they can be built before doing parallel
#  makes for cc1 or cc1plus.  Otherwise sequent parallel make attempts to load
#  them before rtl.o is compiled.
GEN= genemit genoutput genrecog genextract genflags gencodes genconfig genpeep

# Files to be copied away after each stage in building.
STAGESTUFF = *.o insn-flags.h insn-config.h insn-codes.h \
 insn-output.c insn-recog.c insn-emit.c insn-extract.c insn-peep.c \
 insn-attr.h insn-attrtab.c \
 stamp-flags stamp-config stamp-codes \
 stamp-output stamp-recog stamp-emit stamp-extract stamp-peep \
 stamp-attr stamp-attrtab stamp-proto \
 genemit genoutput genrecog genextract genflags gencodes genconfig genpeep \
 genattrtab genattr \
 $(GCC_PASSES) $(EXTRA_PARTS) $(EXTRA_PROGRAMS) gcc-cross cccp \
 cc1plus cc1obj enquire protoize unprotoize specs collect2 $(USE_COLLECT2)

# Members of libgcc1.a.
LIB1FUNCS = _mulsi3 _udivsi3 _divsi3 _umodsi3 _modsi3 \
   _lshrsi3 _lshlsi3 _ashrsi3 _ashlsi3 \
   _divdf3 _muldf3 _negdf2 _adddf3 _subdf3 \
   _fixdfsi _fixsfsi _floatsidf _floatsisf _truncdfsf2 _extendsfdf2 \
   _addsf3 _negsf2 _subsf3 _mulsf3 _divsf3 \
   _eqdf2 _nedf2 _gtdf2 _gedf2 _ltdf2 _ledf2 \
   _eqsf2 _nesf2 _gtsf2 _gesf2 _ltsf2 _lesf2

# Library members defined in libgcc2.c.
LIB2FUNCS = _muldi3 _divdi3 _moddi3 _udivdi3 _umoddi3 _negdi2 \
     _lshrdi3 _lshldi3 _ashldi3 _ashrdi3  \
    _udiv_w_sdiv _udivmoddi4 _cmpdi2 _ucmpdi2 _floatdidf _floatdisf \
    _fixunsdfsi _fixunssfsi _fixunsdfdi _fixdfdi _fixunssfdi _fixsfdi \
    __gcc_bcmp _varargs _eprintf _builtin_new _caps_New _builtin_del \
    _bb _shtab _clear_cache _trampoline __main _exit _ctors

# Header files that are made available under the same name
# to programs compiled with GCC.
USER_H = va-alpha.h va-i860.h va-i960.h va-mips.h va-m88k.h \
    va-pa.h va-pyr.h va-sparc.h va-spur.h proto.h $(EXTRA_HEADERS)

# All the header files that are installed for users from GCC itself.
INSTALLED_H = float.h stddef.h stdarg.h varargs.h $(USER_H) limits.h \
    syslimits.h

# The files that "belong" in CONFIG_H are deliberately omitted
# because having them there would not be useful in actual practice.
# All they would do is cause complete recompilation every time
# one of the machine description files is edited.
# That may or may not be what one wants to do.
# If it is, rm *.o is an easy way to do it.
# CONFIG_H = config.h tm.h
CONFIG_H =
RTL_H = rtl.h rtl.def machmode.h machmode.def
TREE_H = tree.h real.h tree.def machmode.h machmode.def
CPLUS_TREE_H = $(TREE_H) cp-tree.h cp-tree.def

# Avoid a lot of time thinking about remaking Makefile.in and *.def.
.SUFFIXES: .in .def

D 2
Makefile: $(srcdir)/Makefile.in $(srcdir)/configure $(srcdir)/version.c \
   $(srcdir)/config/$(xmake_file) $(srcdir)/config/$(tmake_file)
	sh config.status

E 2
all.internal: start.encap rest.encap
# This is what to compile if making a cross-compiler.
all.cross: native gcc-cross specs libgcc.a cross-test $(EXTRA_PARTS)
# This is what to compile if making gcc with a cross-compiler.
all.build: native xgcc $(EXTRA_PARTS)
# This is what must be made before installing GCC and converting libraries.
start.encap: native xgcc specs $(LIBGCC1) xlimits.h
# Use this to make a GCC that will be used only to recompile GCC.
for-bootstrap: start.encap libgcc.a
# These can't be made, with COFF encapsulation, until after GCC can run.
rest.encap: libgcc.a float.h  $(EXTRA_PARTS)
# This is what is made with the host's compiler
# whether making a cross compiler or not.
native: config.status cpp $(LANGUAGES) $(EXTRA_PASSES) $(EXTRA_PROGRAMS) $(USE_COLLECT2)

# Define the names for selecting languages in LANGUAGES.
C c: cc1
C++ c++: cc1plus
OBJC objc: cc1obj objc-runtime
OBJECTIVE-C objective-c: cc1obj objc-runtime
PROTO: proto

# Really, really stupid make features, such as SUN's KEEP_STATE, may force
# a target to build even if it is up-to-date.  So we must verify that
# config.status does not exist before failing.
config.status:
	@if [ ! -f config.status ] ; then \
	  echo You must configure gcc.  Look at the INSTALL file for details.; \
	  false; \
	else \
	  true; \
	fi

# On the target machine, finish building a cross compiler.
# This does the things that can't be done on the host machine.
rest.cross: libgcc.a float.h specs

# Verify that it works to compile and link cross-test.
# If it does, then there are sufficient replacements for libgcc1.a.
cross-test: cross-test.o native gcc-cross libgcc.a
	$(GCC_FOR_TARGET) $(GCC_CFLAGS) cross-test.o -o $@
cross-test.o: cross-test.c native gcc-cross
	$(GCC_FOR_TARGET) $(GCC_CFLAGS) $(ALL_CPPFLAGS) -c $(srcdir)/cross-test.c

# Recompile all the language-independent object files.
# This is used only if the user explicitly asks for it.
compilations: ${OBJS}

# We call this executable `xgcc' rather than `gcc'
# to avoid confusion if the current directory is in the path
# and CC is `gcc'.  It is renamed to `gcc' when it is installed.
xgcc: gcc.o version.o $(LIBDEPS)
	$(CC) $(ALL_CFLAGS) $(LDFLAGS) -o xgcc gcc.o version.o $(LIBS)

# Dump a specs file to make -B./ read these specs over installed ones.
specs: xgcc
	./xgcc -dumpspecs > specs

# We do want to create an executable named `xgcc', so we can use it to
# compile libgcc2.a.
# Also create gcc-cross, so that install-common will install properly.
gcc-cross: xgcc
	cp xgcc gcc-cross

cc1:$(P) $(C_OBJS) $(OBJS) $(LIBDEPS)
	$(CC) $(ALL_CFLAGS) $(LDFLAGS) -o cc1 $(C_OBJS) $(OBJS) $(LIBS)

cc1plus:$(P) $(CPLUS_OBJS) $(OBJS) $(LIBDEPS)
	$(CC) $(ALL_CFLAGS) $(LDFLAGS) -o cc1plus $(CPLUS_OBJS) $(OBJS) $(LIBS)

cc1obj:$(P) $(OBJC_OBJS) $(OBJS) $(LIBDEPS)
	$(CC) $(ALL_CFLAGS) $(LDFLAGS) -o cc1obj $(OBJC_OBJS) $(OBJS) $(LIBS)

float.h: enquire
	-./enquire -f > tmp-float.h
	mv tmp-float.h float.h

# Used to compile enquire with standard cc, but have forgotten why.
# Let's try with GCC.
enquire: enquire.o $(GCC_PARTS)
	$(GCC_FOR_TARGET) $(GCC_CFLAGS) $(ENQUIRE_LDFLAGS) enquire.o -o $@
enquire.o: $(srcdir)/enquire.c $(GCC_PASSES)
#	-if [ "$(srcdir)" != "." ]; then rm -f ./enquire.c; else true; fi
#	-cp $(srcdir)/enquire.c . > /dev/null 2>&1
# Breaking this line caused a problem with one version of GNU make.
	$(GCC_FOR_TARGET) $(GCC_CFLAGS) $(ALL_CPPFLAGS) $(ENQUIRE_CFLAGS) -I. -c $(srcdir)/enquire.c

# Build the version of limits.h that we will install.
xlimits.h: glimits.h limitx.h limity.h
	if [ -f $(SYSTEM_HEADER_DIR)/limits.h ] ; then \
	  cat $(srcdir)/limitx.h $(srcdir)/glimits.h $(srcdir)/limity.h > xlimits.h; \
	else \
	  cat $(srcdir)/glimits.h > xlimits.h; \
	fi

# Build libgcc.a.
# This is done in two parts because some functions, in libgcc1.c,
# must be compiled with something other than GCC,
# while the rest, in libgcc2.c, must be compiled with xgcc.
# That means we can't do libgcc2.c until after xgcc, cc1, etc.

# Use this as value of LIBGCC1 to cause conversion to GNU library format.
# LIBCONVERT should put its output in libgcc1.conv.
libgcc1.conv: libgcc1.a
	$(LIBCONVERT) libgcc1.a libgcc1.conv

# Use this as value of LIBGCC1 to inhibit use of libgcc1.c entirely.
# Make an empty file instead.
libgcc1.null: $(CROSS_TOOLS) $(GCC_PASSES)
	echo "__foo () {}" > dummy.c
	$(GCC_FOR_TARGET) $(GCC_CFLAGS) -c dummy.c
	$(OLDAR) $(OLDAR_FLAGS) libgcc1.null dummy.o
	rm -f dummy.o dummy.c

# This is $(LIBGCC1) for a cross-compiler.
# We have no automatic way of building libgcc1.a, 
# so it's up to the installer to find a way to do that.
# This rule deliberately does not depend on libgcc1.a
# so that it will fail if the installer hasn't provided it.
libgcc1.cross:
	mv libgcc1.a libgcc1.cross || (echo You must find a way to make libgcc1.a; false)

# Compile the library of arithmetic subroutines with the native compiler.
# Don't compile it with GCC!
# (That would cause most arithmetic functions to call themselves.)
libgcc1.a: libgcc1.c $(CONFIG_H) $(LIB1FUNCS_EXTRA) config.status
	-rm -f tmplibgcc1.a
# Actually build it in tmplibgcc1.a, then rename at end,
# so that libgcc1.a itself remains nonexistent if compilation is aborted.
# -e causes any failing command to make this rule fail.
# -e doesn't work in certain shells, so we test $$? as well.
	set -e; \
	for name in $(LIB1FUNCS); \
	do \
	  echo $${name}; \
	  rm -f $${name}.o; \
	  $(OLDCC) $(CCLIBFLAGS) $(INCLUDES) -c -DL$${name} $(srcdir)/libgcc1.c; \
	  if [ $$? -eq 0 ] ; then true; else exit 1; fi; \
	  mv libgcc1.o $${name}.o; \
	  $(OLDAR) $(OLDAR_FLAGS) tmplibgcc1.a $${name}.o; \
	  rm -f $${name}.o; \
	done
# Some shells crash when a loop has no items.
# So make sure there is always at least one--`..'.
# Then ignore it.
# We don't use -e here because there are if statements
# that should not make the command give up when the if condition is false.
# Instead, we test for failure after each command where it matters.
	-for file in .. $(LIB1FUNCS_EXTRA); \
	do \
	  if [ x$${file} != x.. ]; then \
	    name=`echo $${file} | sed -e 's/[.]c$$//' -e 's/[.]asm$$//'`; \
	    echo $${name}; \
	    if [ $${name}.asm = $${file} ]; then \
	      cp $${file} $${name}.s || exit 1; file=$${name}.s; \
	    else true; fi; \
	    $(OLDCC) $(CCLIBFLAGS) $(INCLUDES) -c $${file}; \
	    if [ $$? -eq 0 ] ; then true; else exit 1; fi; \
	    $(OLDAR) $(OLDAR_FLAGS) tmplibgcc1.a $${name}.o; \
	    if [ $$? -eq 0 ] ; then true; else exit 1; fi; \
	    rm -f $${name}.[so]; \
	  else true; \
	  fi; \
	done
	mv tmplibgcc1.a libgcc1.a

# Compiling libgcc2.a requires making sure that cc1, etc. have been compiled.
# But recompiling cc1 should not force recompilation of libgcc2.a.
# If you want to force recompilation, delete libgcc2.a.
# Depend on install-cross-tools to make sure we set up properly 
# to run the assembler before we try compiling anything.
# install-cross-tools does nothing if not cross compiling.
libgcc2.ready: $(GCC_PASSES) $(CROSS_TOOLS)
	-if [ -f libgcc2.ready ] ; then \
		true; \
	else \
		touch libgcc2.ready; \
	fi

libgcc2.a: libgcc2.c libgcc2.ready $(CONFIG_H) $(LIB2FUNCS_EXTRA) \
   machmode.h longlong.h gbl-ctors.h config.status
# Actually build it in tmplibgcc2.a, then rename at end,
# so that libgcc2.a itself remains nonexistent if compilation is aborted.
	-rm -f tmplibgcc2.a
# -e causes any failing command to make this rule fail.
# -e doesn't work in certain shells, so we test $$? as well.
	set -e; \
	for name in $(LIB2FUNCS); \
	do \
	  echo $${name}; \
	  $(GCC_FOR_TARGET) $(LIBGCC2_CFLAGS) $(INCLUDES) -c -DL$${name} \
	      $(srcdir)/libgcc2.c -o $${name}.o; \
	  if [ $$? -eq 0 ] ; then true; else exit 1; fi; \
	  $(AR) $(AR_FLAGS) tmplibgcc2.a $${name}.o; \
	  rm -f $${name}.o; \
	done
# Some shells crash when a loop has no items.
# So make sure there is always at least one--`..'.
# Then ignore it.
# We don't use -e here because there are if statements
# that should not make the command give up when the if condition is false.
# Instead, we test for failure after each command where it matters.
	-for file in .. $(LIB2FUNCS_EXTRA); \
	do \
	  if [ x$${file} != x.. ]; then \
	    name=`echo $${file} | sed -e 's/[.]c$$//' -e 's/[.]asm$$//'`; \
	    echo $${name}; \
	    if [ $${name}.asm = $${file} ]; then \
	      cp $${file} $${name}.s || exit 1; file=$${name}.s; \
	    else true; fi; \
	    $(GCC_FOR_TARGET) $(LIBGCC2_CFLAGS) $(INCLUDES) -c $${file}; \
	    if [ $$? -eq 0 ] ; then true; else exit 1; fi; \
	    $(AR) $(AR_FLAGS) tmplibgcc2.a $${name}.o; \
	    rm -f $${name}.[so]; \
	  else true; \
	  fi; \
	done
	mv tmplibgcc2.a libgcc2.a
# These lines were deleted from above the mv command
# because ranlibing libgcc.a itself should suffice.
#	-if [ x${HPUX_GAS} = x ] ; then \
#	  if $(RANLIB_TEST) ; then $(RANLIB) tmplibgcc2.a; else true; fi \
#	else true; fi

# Combine the various libraries into a single library, libgcc.a.
libgcc.a: $(LIBGCC1) $(LIBGCC2)
	-rm -rf tmplibgcc.a libgcc.a tmpcopy
	mkdir tmpcopy
	-if [ x$(LIBGCC1) != x ];			\
	then (cd tmpcopy; $(AR) x ../$(LIBGCC1));	\
	else true;					\
	fi
	(cd tmpcopy; $(AR) x ../$(LIBGCC2))
	(cd tmpcopy; $(AR) $(AR_FLAGS) ../tmplibgcc.a *.o)
	rm -rf tmpcopy
	-if $(RANLIB_TEST) ; then $(RANLIB) tmplibgcc.a; else true; fi
# Actually build it in tmplibgcc.a, then rename at end,
# so that libgcc.a itself remains nonexistent if compilation is aborted.
	mv tmplibgcc.a libgcc.a

objc-runtime: libobjc.a

# Build the Objective C runtime library.
libobjc.a: cc1obj libgcc2.ready
	if [ -d objc ]; then true; else mkdir objc; fi
	thisdir1=`pwd`; \
	srcdir1=`cd $(srcdir); pwd`; \
	cd objc; \
	$(MAKE) -f $${srcdir1}/objc/Makefile libobjc.a \
	  srcdir=$${srcdir1} tooldir=$(tooldir) AR="$(AR)" AR_FLAGS="$(AR_FLAGS)" \
	  GCC_FOR_TARGET="$${thisdir1}/xgcc -B$${thisdir1}/" \
	  GCC_CFLAGS="$(GCC_CFLAGS)"
	-rm -f libobjc.a
	ln objc/libobjc.a . >/dev/null 2>&1 || cp objc/libobjc.a .
	-if $(RANLIB_TEST) ; then $(RANLIB) libobjc.a; else true; fi

# This is used by objc/Makefile if the user runs that directly.
sublibobjc.a: cc1obj libgcc2.ready
	thisdir1=`pwd`; \
	srcdir1=`cd $(srcdir); pwd`; \
	cd objc; \
	$(MAKE) -f $$srcdir1/objc/Makefile libobjc.a \
	  srcdir=$$srcdir1 tooldir=$(tooldir) AR="$(AR)" AR_FLAGS="$(AR_FLAGS)" \
	  GCC_FOR_TARGET="$$thisdir1/xgcc -B$$thisdir1/" \
	  GCC_CFLAGS="$(GCC_CFLAGS)"

# Compile two additional files that are linked with every program
# linked using GCC on system V, for the sake of C++ constructors.
crtbegin.o:	crtstuff.c $(GCC_PASSES) $(CONFIG_H) gbl-ctors.h
	$(GCC_FOR_TARGET) $(GCC_CFLAGS) $(INCLUDES) -DCRT_BEGIN \
	  -finhibit-size-directive -g0 -c $(srcdir)/crtstuff.c -o crtbegin.o

crtend.o:	crtstuff.c $(GCC_PASSES) $(CONFIG_H) gbl-ctors.h
	$(GCC_FOR_TARGET) $(GCC_CFLAGS) $(INCLUDES) -DCRT_END \
	  -finhibit-size-directive -g0 -c $(srcdir)/crtstuff.c -o crtend.o

# Compiling object files from source files.

# Note that dependencies on obstack.h are not written
# because that file is not part of GCC.
# Dependencies on gvarargs.h are not written
# because all that file does, when not compiling with GCC,
# is include the system varargs.h.

# C language specific files.

c-parse.o : $(srcdir)/c-parse.c $(CONFIG_H) $(TREE_H) c-lex.h c-parse.h \
    c-tree.h input.h flags.h
	$(CC) $(ALL_CFLAGS) $(ALL_CPPFLAGS) $(INCLUDES) -c $(srcdir)/c-parse.c
$(srcdir)/c-parse.c $(srcdir)/c-parse.h: $(srcdir)/c-parse.y
	cd $(srcdir); $(BISON) $(BISONFLAGS) -d c-parse.y -o c-parse.c
$(srcdir)/c-parse.y: $(srcdir)/c-parse.in $(srcdir)/cond.awk
	cd $(srcdir); awk -f cond.awk objc=0 c-parse.in > c-parse.y

c-decl.o : c-decl.c $(CONFIG_H) $(TREE_H) c-tree.h c-lex.h flags.h
c-typeck.o : c-typeck.c $(CONFIG_H) $(TREE_H) c-tree.h flags.h
c-lang.o : c-lang.c $(CONFIG_H) $(TREE_H)
c-lex.o : c-lex.c $(CONFIG_H) $(TREE_H) c-lex.h c-tree.h c-parse.h \
    input.h flags.h
c-aux-info.o : c-aux-info.c  $(CONFIG_H) $(TREE_H) c-tree.h flags.h
c-convert.o : c-convert.c $(CONFIG_H) $(TREE_H) flags.h
c-pragma.o: c-pragma.c $(CONFIG_H) $(TREE_H)

# C++ language specific files.

cp-parse.o : $(srcdir)/cp-parse.c $(CONFIG_H) $(CPLUS_TREE_H) flags.h
	$(CC) -c $(ALL_CFLAGS) $(ALL_CPPFLAGS) $(INCLUDES) \
  `echo $(srcdir)/cp-parse.c | sed 's,^\./,,'`

$(srcdir)/cp-parse.c $(srcdir)/cp-parse.h : $(srcdir)/cp-parse.y
	@echo expect 29 shift/reduce conflicts and 14 reduce/reduce conflicts
	cd $(srcdir); $(BISON) $(BISONFLAGS) -d -o cp-parse.c cp-parse.y
	cd $(srcdir); grep '^#define[ 	]*YYEMPTY' cp-parse.c >>cp-parse.h

cp-spew.o : cp-spew.c $(CONFIG_H) $(CPLUS_TREE_H) \
   $(srcdir)/cp-parse.h flags.h
cp-lex.o : cp-lex.c $(CONFIG_H) $(CPLUS_TREE_H) \
   $(srcdir)/cp-parse.h $(srcdir)/cp-input.c flags.h
cp-decl.o : cp-decl.c $(CONFIG_H) $(CPLUS_TREE_H) flags.h \
  cp-lex.h cp-decl.h stack.h
cp-decl2.o : cp-decl2.c $(CONFIG_H) $(CPLUS_TREE_H) flags.h \
  cp-lex.h cp-decl.h
cp-type2.o : cp-type2.c $(CONFIG_H) $(CPLUS_TREE_H) flags.h
cp-typeck.o : cp-typeck.c $(CONFIG_H) $(CPLUS_TREE_H) flags.h
cp-class.o : cp-class.c $(CONFIG_H) $(CPLUS_TREE_H) stack.h \
    cp-class.h flags.h
cp-call.o : cp-call.c $(CONFIG_H) $(CPLUS_TREE_H) stack.h cp-class.h \
    flags.h
cp-init.o : cp-init.c $(CONFIG_H) $(CPLUS_TREE_H) flags.h
cp-method.o : cp-method.c $(CONFIG_H) $(CPLUS_TREE_H)
cp-cvt.o : cp-cvt.c $(CONFIG_H) $(CPLUS_TREE_H)
cp-search.o : cp-search.c $(CONFIG_H) $(CPLUS_TREE_H) stack.h flags.h
cp-tree.o : cp-tree.c $(CONFIG_H) $(CPLUS_TREE_H) flags.h
cp-ptree.o : cp-ptree.c $(CONFIG_H) $(CPLUS_TREE_H)
cp-gc.o : cp-gc.c $(CONFIG_H) $(CPLUS_TREE_H) flags.h
cp-except.o : cp-except.c $(CONFIG_H) $(CPLUS_TREE_H) flags.h
cp-expr.o : cp-expr.c $(CONFIG_H) $(CPLUS_TREE_H) $(RTL_H) flags.h \
  expr.h insn-codes.h
cp-edsel.o : cp-edsel.c $(CONFIG_H) $(CPLUS_TREE_H) stack.h flags.h
cp-xref.o : cp-xref.c $(CONFIG_H) $(CPLUS_TREE_H)
cp-pt.o : cp-pt.c $(CONFIG_H) $(CPLUS_TREE_H) cp-decl.h cp-parse.h

# To make a configuration always use collect2, set USE_COLLECT2 to ld.
ld: collect2
	rm -f ld
	ln collect2 ld

collect2 : collect2.o version.o $(LIBDEPS)
# Don't try modifying collect2 (aka ld) in place--it might be linking this.
	-rm -f collect2
	$(CC) $(ALL_CFLAGS) $(LDFLAGS) -o collect2 collect2.o version.o $(LIBS)

#  -DSTANDARD_EXEC_PREFIX=\"$(libdir)/gcc-lib/\" 
collect2.o : collect2.c $(CONFIG_H) gstab.h
	$(CC) $(ALL_CFLAGS) $(ALL_CPPFLAGS) $(INCLUDES) \
  -DSTANDARD_STARTFILE_PREFIX=\"$(libdir)/\" \
  -DSTANDARD_BIN_PREFIX=\"$(bindir)/\" \
  -DSTANDARD_EXEC_PREFIX=\"/usr/libexec/gcc2/\" \
  -DDEFAULT_TARGET_MACHINE=\"$(target)\" \
  -c `echo $(srcdir)/collect2.c | sed 's,^\./,,'`

# Objectionable C language specific files.

objc-parse.o : $(srcdir)/objc-parse.c $(CONFIG_H) $(TREE_H) c-lex.h \
   c-tree.h input.h flags.h objc-actions.h
	$(CC) $(ALL_CFLAGS) $(ALL_CPPFLAGS) $(INCLUDES) -c $(srcdir)/objc-parse.c
$(srcdir)/objc-parse.c : $(srcdir)/objc-parse.y
	cd $(srcdir); $(BISON) $(BISONFLAGS) objc-parse.y -o objc-parse.c
$(srcdir)/objc-parse.y: $(srcdir)/c-parse.in $(srcdir)/cond.awk
	cd $(srcdir); awk -f cond.awk objc=1 c-parse.in > objc-parse.y

objc-actions.o : objc-actions.c $(CONFIG_H) $(TREE_H) c-tree.h c-lex.h \
   flags.h objc-actions.h

# A file used by all variants of C.

c-common.o : c-common.c $(CONFIG_H) $(TREE_H) c-tree.h c-lex.h flags.h

# Language-independent files.

#  -DSTANDARD_EXEC_PREFIX=\"$(libdir)/gcc-lib/\"
gcc.o: gcc.c $(CONFIG_H) gvarargs.h obstack.h
	$(CC) $(ALL_CFLAGS) $(ALL_CPPFLAGS) $(INCLUDES) \
  -DSTANDARD_STARTFILE_PREFIX=\"$(libdir)/\" \
   -DSTANDARD_EXEC_PREFIX=\"/usr/libexec/gcc2/\" \
  -DDEFAULT_TARGET_MACHINE=\"$(target)\" \
  -DTOOLDIR=\"$(tooldir)/\" \
  -c `echo $(srcdir)/gcc.c | sed 's,^\./,,'`

dumpvers: dumpvers.c

version.o: version.c
obstack.o: obstack.c

tree.o : tree.c $(CONFIG_H) $(TREE_H) gvarargs.h flags.h function.h
print-tree.o : print-tree.c $(CONFIG_H) $(TREE_H)
stor-layout.o : stor-layout.c $(CONFIG_H) $(TREE_H) function.h
fold-const.o : fold-const.c $(CONFIG_H) $(TREE_H) flags.h 
toplev.o : toplev.c $(CONFIG_H) $(TREE_H) $(RTL_H) flags.h input.h \
   insn-attr.h xcoffout.h
	$(CC) $(ALL_CFLAGS) $(ALL_CPPFLAGS) $(INCLUDES) \
	  $(MAYBE_TARGET_DEFAULT) $(MAYBE_USE_COLLECT2) \
	  -c `echo $(srcdir)/toplev.c | sed 's,^\./,,'`

rtl.o : rtl.c $(CONFIG_H) $(RTL_H)

print-rtl.o : print-rtl.c $(CONFIG_H) $(RTL_H)
rtlanal.o : rtlanal.c $(CONFIG_H) $(RTL_H)

varasm.o : varasm.c $(CONFIG_H) $(TREE_H) $(RTL_H) flags.h function.h \
   defaults.h insn-codes.h expr.h hard-reg-set.h regs.h xcoffout.h
function.o : function.c $(CONFIG_H) $(RTL_H) $(TREE_H) flags.h function.h  \
   insn-flags.h insn-codes.h expr.h regs.h hard-reg-set.h insn-config.h \
   recog.h output.h
stmt.o : stmt.c $(CONFIG_H) $(RTL_H) $(TREE_H) flags.h function.h  \
   insn-flags.h insn-config.h insn-codes.h hard-reg-set.h expr.h loop.h recog.h
expr.o : expr.c $(CONFIG_H) $(RTL_H) $(TREE_H) gvarargs.h flags.h function.h  \
   insn-flags.h insn-codes.h expr.h insn-config.h recog.h output.h typeclass.h
calls.o : calls.c $(CONFIG_H) $(RTL_H) $(TREE_H) flags.h expr.h insn-codes.h \
   insn-flags.h
expmed.o : expmed.c $(CONFIG_H) $(RTL_H) $(TREE_H) flags.h  \
   insn-flags.h insn-config.h insn-codes.h expr.h recog.h real.h
explow.o : explow.c $(CONFIG_H) $(RTL_H) $(TREE_H) flags.h hard-reg-set.h \
   insn-config.h expr.h recog.h insn-flags.h insn-codes.h
optabs.o : optabs.c $(CONFIG_H) $(RTL_H) $(TREE_H) flags.h  \
   insn-flags.h insn-config.h insn-codes.h expr.h recog.h
dbxout.o : dbxout.c $(CONFIG_H) $(TREE_H) $(RTL_H) flags.h regs.h \
   insn-config.h reload.h gstab.h xcoffout.h defaults.h output.h
sdbout.o : sdbout.c $(CONFIG_H) $(TREE_H) $(RTL_H) gsyms.h flags.h \
   insn-config.h reload.h
dwarfout.o : dwarfout.c $(CONFIG_H) $(TREE_H) $(RTL_H) dwarf.h flags.h \
   insn-config.h reload.h output.h defaults.h
xcoffout.o : xcoffout.c $(CONFIG_H) $(TREE_H) $(RTL_H) xcoffout.h flags.h
emit-rtl.o : emit-rtl.c $(CONFIG_H) $(RTL_H) flags.h gvarargs.h function.h  \
   regs.h insn-config.h insn-codes.h real.h expr.h
getpwd.o : getpwd.c $(CONFIG_H)

integrate.o : integrate.c $(CONFIG_H) $(RTL_H) $(TREE_H) flags.h integrate.h \
   insn-flags.h insn-config.h insn-codes.h expr.h real.h function.h

jump.o : jump.c $(CONFIG_H) $(RTL_H) flags.h hard-reg-set.h regs.h \
   insn-config.h insn-flags.h insn-codes.h expr.h real.h
stupid.o : stupid.c $(CONFIG_H) $(RTL_H) regs.h hard-reg-set.h flags.h

cse.o : cse.c $(CONFIG_H) $(RTL_H) regs.h hard-reg-set.h flags.h real.h \
   insn-config.h recog.h
loop.o : loop.c $(CONFIG_H) $(RTL_H) flags.h loop.h insn-config.h \
   insn-flags.h insn-codes.h regs.h hard-reg-set.h recog.h expr.h real.h
unroll.o : unroll.c $(CONFIG_H) $(RTL_H) insn-config.h insn-codes.h \
   integrate.h regs.h flags.h expr.h loop.h
flow.o : flow.c $(CONFIG_H) $(RTL_H) flags.h insn-config.h \
   basic-block.h regs.h hard-reg-set.h output.h
combine.o : combine.c $(CONFIG_H) $(RTL_H) gvarargs.h flags.h  \
   insn-config.h insn-flags.h insn-codes.h insn-attr.h regs.h expr.h \
   basic-block.h recog.h real.h
regclass.o : regclass.c $(CONFIG_H) $(RTL_H) hard-reg-set.h flags.h \
   basic-block.h regs.h insn-config.h recog.h reload.h real.h
local-alloc.o : local-alloc.c $(CONFIG_H) $(RTL_H) flags.h basic-block.h \
   regs.h hard-reg-set.h insn-config.h recog.h output.h
global.o : global.c $(CONFIG_H) $(RTL_H) flags.h  \
   basic-block.h regs.h hard-reg-set.h insn-config.h output.h

reload.o : reload.c $(CONFIG_H) $(RTL_H) flags.h \
   reload.h recog.h hard-reg-set.h insn-config.h insn-codes.h regs.h real.h
reload1.o : reload1.c $(CONFIG_H) $(RTL_H) flags.h expr.h \
   reload.h regs.h hard-reg-set.h insn-config.h insn-flags.h insn-codes.h \
   basic-block.h recog.h output.h
caller-save.o : caller-save.c $(CONFIG_H) $(RTL_H) flags.h \
   regs.h hard-reg-set.h insn-config.h basic-block.h recog.h reload.h expr.h
reorg.o : reorg.c $(CONFIG_H) $(RTL_H) conditions.h hard-reg-set.h \
   basic-block.h regs.h insn-config.h insn-attr.h insn-flags.h recog.h \
   flags.h output.h
sched.o : sched.c $(CONFIG_H) $(RTL_H) basic-block.h regs.h hard-reg-set.h \
   flags.h insn-config.h insn-attr.h
final.o : final.c $(CONFIG_H) $(RTL_H) gvarargs.h flags.h regs.h \
   recog.h conditions.h insn-config.h insn-attr.h real.h output.h \
   hard-reg-set.h insn-flags.h insn-codes.h gstab.h xcoffout.h defaults.h
recog.o : recog.c $(CONFIG_H) $(RTL_H)  \
   regs.h recog.h hard-reg-set.h flags.h insn-config.h insn-attr.h \
   insn-flags.h insn-codes.h real.h
reg-stack.o : reg-stack.c $(CONFIG_H) $(RTL_H) $(TREE_H) \
   regs.h hard-reg-set.h flags.h insn-config.h

aux-output.o : aux-output.c $(CONFIG_H) \
   $(RTL_H) regs.h hard-reg-set.h real.h insn-config.h conditions.h \
   insn-flags.h output.h insn-attr.h insn-codes.h

# Normally this target is not used; but it is used if you
# define ALLOCA=alloca.o.  In that case, you must get a suitable alloca.c
# from the GNU Emacs distribution.
alloca.o:	alloca.c
	$(CC) $(ALL_CFLAGS) $(ALL_CPPFLAGS) $(INCLUDES) $(ALLOCA_FLAGS) \
	  -Demacs -c `echo $(srcdir)/alloca.c | sed 's,^\./,,'`
	$(ALLOCA_FINISH)

# Generate header and source files from the machine description, 
# and compile them.

.PRECIOUS: insn-config.h insn-flags.h insn-codes.h \
  insn-emit.c insn-recog.c insn-extract.c insn-output.c insn-peep.c \
  insn-attr.h insn-attrtab.c

# The following pair of rules has this effect:
# genconfig is run only if the md has changed since genconfig was last run;
# but the file insn-config.h is touched only when its contents actually change.

# Each of the other insn-* files is handled by a similar pair of rules.

# This causes an anomaly in the results of make -n
# because insn-* is older than stamp-*
# and thus make -n thinks that insn-* will be updated
# and force recompilation of things that depend on it.
# We use move-if-changed precisely to avoid such recompilation.
# But there is no way to teach make -n that it will be avoided.

# Each of the insn-*.[ch] rules has a semicolon at the end,
# for otherwise the system Make on SunOS 4.1 never tries
# to recompile insn-*.o.

insn-config.h: stamp-config ;
stamp-config : md genconfig $(srcdir)/move-if-change
	./genconfig md > tmp-config.h
	$(srcdir)/move-if-change tmp-config.h insn-config.h
	touch stamp-config

insn-flags.h: stamp-flags ;
stamp-flags : md genflags $(srcdir)/move-if-change
	./genflags md > tmp-flags.h
	$(srcdir)/move-if-change tmp-flags.h insn-flags.h
	touch stamp-flags

insn-codes.h: stamp-codes ;
stamp-codes : md gencodes $(srcdir)/move-if-change
	./gencodes md > tmp-codes.h
	$(srcdir)/move-if-change tmp-codes.h insn-codes.h
	touch stamp-codes

insn-emit.o : insn-emit.c $(CONFIG_H) $(RTL_H) expr.h real.h output.h \
  insn-config.h insn-flags.h insn-codes.h
	$(CC) $(ALL_CFLAGS) $(ALL_CPPFLAGS) $(INCLUDES) -c insn-emit.c

insn-emit.c: stamp-emit ;
stamp-emit : md genemit $(srcdir)/move-if-change
	./genemit md > tmp-emit.c
	$(srcdir)/move-if-change tmp-emit.c insn-emit.c
	touch stamp-emit

insn-recog.o : insn-recog.c $(CONFIG_H) $(RTL_H) insn-config.h recog.h \
  real.h output.h flags.h
	$(CC) $(ALL_CFLAGS) $(ALL_CPPFLAGS) $(INCLUDES) -c insn-recog.c

insn-recog.c: stamp-recog ;
stamp-recog : md genrecog $(srcdir)/move-if-change
	./genrecog md > tmp-recog.c
	$(srcdir)/move-if-change tmp-recog.c insn-recog.c
	touch stamp-recog

insn-extract.o : insn-extract.c $(CONFIG_H) $(RTL_H)
	$(CC) $(ALL_CFLAGS) $(ALL_CPPFLAGS) $(INCLUDES) -c insn-extract.c

insn-extract.c: stamp-extract ;
stamp-extract : md genextract $(srcdir)/move-if-change
	./genextract md > tmp-extract.c
	$(srcdir)/move-if-change tmp-extract.c insn-extract.c
	touch stamp-extract

insn-peep.o : insn-peep.c $(CONFIG_H) $(RTL_H) regs.h output.h real.h
	$(CC) $(ALL_CFLAGS) $(ALL_CPPFLAGS) $(INCLUDES) -c insn-peep.c

insn-peep.c: stamp-peep ;
stamp-peep : md genpeep $(srcdir)/move-if-change
	./genpeep md > tmp-peep.c
	$(srcdir)/move-if-change tmp-peep.c insn-peep.c
	touch stamp-peep

insn-attrtab.o : insn-attrtab.c $(CONFIG_H) $(RTL_H) regs.h real.h output.h \
     insn-attr.h insn-config.h
	$(CC) $(ALL_CFLAGS) $(ALL_CPPFLAGS) $(INCLUDES) -c insn-attrtab.c

insn-attr.h: stamp-attr ;
stamp-attr : md genattr $(srcdir)/move-if-change
	./genattr md > tmp-attr.h
	$(srcdir)/move-if-change tmp-attr.h insn-attr.h
	touch stamp-attr

insn-attrtab.c: stamp-attrtab ;
stamp-attrtab : md genattrtab $(srcdir)/move-if-change
	if cmp -s $(PREMADE_ATTRTAB_MD) md;	\
	then					\
	  echo Using $(PREMADE_ATTRTAB);	\
	  cp $(PREMADE_ATTRTAB) tmp-attrtab.c;	\
	else					\
	  ./genattrtab md > tmp-attrtab.c;	\
	fi
	$(srcdir)/move-if-change tmp-attrtab.c insn-attrtab.c
	touch stamp-attrtab

insn-output.o : insn-output.c $(CONFIG_H) $(RTL_H) regs.h real.h conditions.h \
    hard-reg-set.h insn-config.h insn-flags.h insn-attr.h output.h recog.h \
    insn-codes.h
	$(CC) $(ALL_CFLAGS) $(ALL_CPPFLAGS) $(INCLUDES) -c insn-output.c

insn-output.c: stamp-output ;
stamp-output : md genoutput $(srcdir)/move-if-change
	./genoutput md > tmp-output.c
	$(srcdir)/move-if-change tmp-output.c insn-output.c
	touch stamp-output

# Compile the programs that generate insn-* from the machine description.
# They are compiled with $(HOST_CC), and associated libraries,
# since they need to run on this machine
# even if GCC is being compiled to run on some other machine.

# $(CONFIG_H) is omitted from the deps of the gen*.o
# because these programs don't really depend on anything 
# about the target machine.  They do depend on config.h itself,
# since that describes the host machine.

genconfig : genconfig.o $(HOST_RTL) $(HOST_LIBDEPS)
	$(HOST_CC) $(HOST_CFLAGS) $(HOST_LDFLAGS) -o genconfig \
	  genconfig.o $(HOST_RTL) $(HOST_LIBS)

genconfig.o : genconfig.c $(RTL_H) hconfig.h
	$(HOST_CC) -c $(HOST_CFLAGS) $(HOST_CPPFLAGS) $(INCLUDES) $(srcdir)/genconfig.c

genflags : genflags.o $(HOST_RTL) $(HOST_LIBDEPS)
	$(HOST_CC) $(HOST_CFLAGS) $(HOST_LDFLAGS) -o genflags \
	 genflags.o $(HOST_RTL) $(HOST_LIBS)

genflags.o : genflags.c $(RTL_H) hconfig.h
	$(HOST_CC) -c $(HOST_CFLAGS) $(HOST_CPPFLAGS) $(INCLUDES) $(srcdir)/genflags.c

gencodes : gencodes.o $(HOST_RTL) $(HOST_LIBDEPS)
	$(HOST_CC) $(HOST_CFLAGS) $(HOST_LDFLAGS) -o gencodes \
	 gencodes.o $(HOST_RTL) $(HOST_LIBS)

gencodes.o : gencodes.c $(RTL_H) hconfig.h
	$(HOST_CC) -c $(HOST_CFLAGS) $(HOST_CPPFLAGS) $(INCLUDES) $(srcdir)/gencodes.c

genemit : genemit.o $(HOST_RTL) $(HOST_LIBDEPS)
	$(HOST_CC) $(HOST_CFLAGS) $(HOST_LDFLAGS) -o genemit \
	 genemit.o $(HOST_RTL) $(HOST_LIBS)

genemit.o : genemit.c $(RTL_H) hconfig.h
	$(HOST_CC) -c $(HOST_CFLAGS) $(HOST_CPPFLAGS) $(INCLUDES) $(srcdir)/genemit.c

genrecog : genrecog.o $(HOST_RTL) $(HOST_LIBDEPS)
	$(HOST_CC) $(HOST_CFLAGS) $(HOST_LDFLAGS) -o genrecog \
	 genrecog.o $(HOST_RTL) $(HOST_LIBS)

genrecog.o : genrecog.c $(RTL_H) hconfig.h
	$(HOST_CC) -c $(HOST_CFLAGS) $(HOST_CPPFLAGS) $(INCLUDES) $(srcdir)/genrecog.c

genextract : genextract.o $(HOST_RTL) $(HOST_LIBDEPS)
	$(HOST_CC) $(HOST_CFLAGS) $(HOST_LDFLAGS) -o genextract \
	 genextract.o $(HOST_RTL) $(HOST_LIBS)

genextract.o : genextract.c $(RTL_H) hconfig.h insn-config.h
	$(HOST_CC) -c $(HOST_CFLAGS) $(HOST_CPPFLAGS) $(INCLUDES) $(srcdir)/genextract.c

genpeep : genpeep.o $(HOST_RTL) $(HOST_LIBDEPS)
	$(HOST_CC) $(HOST_CFLAGS) $(HOST_LDFLAGS) -o genpeep \
	 genpeep.o $(HOST_RTL) $(HOST_LIBS)

genpeep.o : genpeep.c $(RTL_H) hconfig.h
	$(HOST_CC) -c $(HOST_CFLAGS) $(HOST_CPPFLAGS) $(INCLUDES) $(srcdir)/genpeep.c

genattr : genattr.o $(HOST_RTL) $(HOST_LIBDEPS)
	$(HOST_CC) $(HOST_CFLAGS) $(HOST_LDFLAGS) -o genattr \
	 genattr.o $(HOST_RTL) $(HOST_LIBS)

genattr.o : genattr.c $(RTL_H) hconfig.h
	$(HOST_CC) -c $(HOST_CFLAGS) $(HOST_CPPFLAGS) $(INCLUDES) $(srcdir)/genattr.c

genattrtab : genattrtab.o $(HOST_RTL) $(HOST_PRINT) $(HOST_RTLANAL) $(HOST_LIBDEPS)
	$(HOST_CC) $(HOST_CFLAGS) $(HOST_LDFLAGS) -o genattrtab \
	 genattrtab.o $(HOST_RTL) $(HOST_PRINT) $(HOST_RTLANAL) $(HOST_LIBS)

genattrtab.o : genattrtab.c $(RTL_H) hconfig.h insn-config.h
	$(HOST_CC) -c $(HOST_CFLAGS) $(HOST_CPPFLAGS) $(INCLUDES) $(srcdir)/genattrtab.c

genoutput : genoutput.o $(HOST_RTL) $(HOST_LIBDEPS)
	$(HOST_CC) $(HOST_CFLAGS) $(HOST_LDFLAGS) -o genoutput \
	 genoutput.o $(HOST_RTL) $(HOST_LIBS)

genoutput.o : genoutput.c $(RTL_H) hconfig.h
	$(HOST_CC) -c $(HOST_CFLAGS) $(HOST_CPPFLAGS) $(INCLUDES) $(srcdir)/genoutput.c

# Compile the libraries to be used by gen*.
# If we are not cross-building, gen* use the same .o's that cc1 will use,
# and HOST_PREFIX_1 is `foobar', just to ensure these rules don't conflict
# with the rules for rtl.o, alloca.o, etc.
$(HOST_PREFIX_1)rtl.o: $(srcdir)/rtl.c $(CONFIG_H) $(RTL_H)
	rm -f $(HOST_PREFIX)rtl.c
	sed -e 's/config[.]h/hconfig.h/' $(srcdir)/rtl.c > $(HOST_PREFIX)rtl.c
	$(HOST_CC) -c $(HOST_CFLAGS) $(HOST_CPPFLAGS) $(INCLUDES) $(HOST_PREFIX)rtl.c

$(HOST_PREFIX_1)print-rtl.o: $(srcdir)/print-rtl.c $(CONFIG_H) $(RTL_H)
	rm -f $(HOST_PREFIX)print-rtl.c
	sed -e 's/config[.]h/hconfig.h/' $(srcdir)/print-rtl.c > $(HOST_PREFIX)print-rtl.c
	$(HOST_CC) -c $(HOST_CFLAGS) $(HOST_CPPFLAGS) $(INCLUDES) $(HOST_PREFIX)print-rtl.c

$(HOST_PREFIX_1)rtlanal.o: $(srcdir)/rtlanal.c $(CONFIG_H) $(RTL_H)
	rm -f $(HOST_PREFIX)rtlanal.c
	sed -e 's/config[.]h/hconfig.h/' $(srcdir)/rtlanal.c > $(HOST_PREFIX)rtlanal.c
	$(HOST_CC) -c $(HOST_CFLAGS) $(HOST_CPPFLAGS) $(INCLUDES) $(HOST_PREFIX)rtlanal.c

$(HOST_PREFIX_1)alloca.o: alloca.c
	rm -f $(HOST_PREFIX)alloca.c
	cp $(srcdir)/alloca.c $(HOST_PREFIX)alloca.c
	$(HOST_CC) -c $(HOST_CFLAGS) $(HOST_CPPFLAGS) $(INCLUDES) $(HOST_PREFIX)alloca.c

$(HOST_PREFIX_1)obstack.o: obstack.c
	rm -f $(HOST_PREFIX)obstack.c
	sed -e 's/config[.]h/hconfig.h/' $(srcdir)/obstack.c > $(HOST_PREFIX)obstack.c
	$(HOST_CC) -c $(HOST_CFLAGS) $(HOST_CPPFLAGS) $(INCLUDES) $(HOST_PREFIX)obstack.c

$(HOST_PREFIX_1)malloc.o: malloc.c
	rm -f $(HOST_PREFIX)malloc.c
	sed -e 's/config[.]h/hconfig.h/' $(srcdir)/malloc.c > $(HOST_PREFIX)malloc.c
	$(HOST_CC) -c $(HOST_CFLAGS) $(HOST_CPPFLAGS) $(INCLUDES) $(HOST_PREFIX)malloc.c

# This satisfies the dependency that we get if you cross-compile a compiler
# that does not need to compile alloca, malloc or whatever.
$(HOST_PREFIX_1): 
	touch $(HOST_PREFIX_1)

# Remake cpp and protoize.

# Making the preprocessor
cpp: cccp
	-rm -f cpp
	ln cccp cpp
cccp: cccp.o cexp.o version.o $(LIBDEPS)
	$(CC) $(ALL_CFLAGS) $(LDFLAGS) -o cccp cccp.o cexp.o version.o $(LIBS)
cexp.o: $(srcdir)/cexp.c $(CONFIG_H)
	$(CC) $(ALL_CFLAGS) $(ALL_CPPFLAGS) $(INCLUDES) -c $(srcdir)/cexp.c
$(srcdir)/cexp.c: $(srcdir)/cexp.y
	cd $(srcdir); $(BISON) -o cexp.c cexp.y
cccp.o: cccp.c $(CONFIG_H) pcp.h version.c
# The reason we use $(libdir)/g++-include rather than using libsubdir
# is for compatibility with the current version of libg++.
	$(CC) $(ALL_CFLAGS) $(ALL_CPPFLAGS) $(INCLUDES) \
	  -DGCC_INCLUDE_DIR=\"$(libsubdir)/include\" \
	  -DGPLUSPLUS_INCLUDE_DIR=\"$(libdir)/g++-include\" \
	  -DLOCAL_INCLUDE_DIR=\"$(prefix)/include\" \
	  -DGPLUSPLUS_INCLUDE_DIR=\"$(libdir)/g++-include\" \
	  -DLOCAL_INCLUDE_DIR=\"$(prefix)/include\" \
	  -DCROSS_INCLUDE_DIR=\"$(libsubdir)/sys-include\" \
	  -DTOOLDIR=\"$(tooldir)/\" \
	  -c `echo $(srcdir)/cccp.c | sed 's,^\./,,'`

proto: config.status protoize unprotoize SYSCALLS.c.X

protoize: protoize.o getopt.o getopt1.o getpwd.o version.o $(LIBDEPS)
	$(CC) $(ALL_CFLAGS) $(LDFLAGS) \
	  protoize.o getopt.o getopt1.o getpwd.o version.o $(LIBS) -o $@
protoize.o: stamp-proto ;

unprotoize: unprotoize.o getopt.o getopt1.o getpwd.o version.o $(LIBDEPS)
	$(CC) $(ALL_CFLAGS) $(LDFLAGS) \
	  unprotoize.o getopt.o getopt1.o getpwd.o version.o $(LIBS) -o $@
unprotoize.o:	stamp-proto ;

stamp-proto:	$(srcdir)/protoize.c getopt.h $(CONFIG_H)
	$(CC) -c $(ALL_CFLAGS) $(ALL_CPPFLAGS) $(INCLUDES) \
          -DGCC_INCLUDE_DIR=\"$(libsubdir)/include\" \
          -DGPLUSPLUS_INCLUDE_DIR=\"$(libdir)/g++-include\" \
          -DCROSS_INCLUDE_DIR=\"$(libsubdir)/sys-include\" \
	  -DLOCAL_INCLUDE_DIR=\"$(prefix)/include\" \
	  -DSTD_PROTO_DIR=\"$(libsubdir)\" \
	  -DUNPROTOIZE $(srcdir)/protoize.c
	mv protoize.o unprotoize.o
	$(CC) -c $(ALL_CFLAGS) $(ALL_CPPFLAGS) $(INCLUDES) \
          -DGCC_INCLUDE_DIR=\"$(libsubdir)/include\" \
          -DGPLUSPLUS_INCLUDE_DIR=\"$(libdir)/g++-include\" \
          -DCROSS_INCLUDE_DIR=\"$(libsubdir)/sys-include\" \
	  -DLOCAL_INCLUDE_DIR=\"$(prefix)/include\" \
	  -DSTD_PROTO_DIR=\"$(libsubdir)\" \
	  $(srcdir)/protoize.c
	touch stamp-proto

getopt.o: $(srcdir)/getopt.c getopt.h
	$(CC) -c $(ALL_CFLAGS) $(ALL_CPPFLAGS) $(INCLUDES) $(srcdir)/getopt.c
getopt1.o: $(srcdir)/getopt1.c getopt.h
	$(CC) -c $(ALL_CFLAGS) $(ALL_CPPFLAGS) $(INCLUDES) $(srcdir)/getopt1.c

# This info describes the target machine, so compile with GCC just built.
SYSCALLS.c.X: $(srcdir)/sys-types.h $(srcdir)/sys-protos.h $(GCC_PASSES)
	-rm -f SYSCALLS.c tmp-SYSCALLS.s
	cat $(srcdir)/sys-types.h $(srcdir)/sys-protos.h > SYSCALLS.c
	$(GCC_FOR_TARGET) $(GCC_CFLAGS) $(ALL_CPPFLAGS) $(INCLUDES) \
	  -aux-info $@ -S -o tmp-SYSCALLS.s SYSCALLS.c
	-rm -f SYSCALLS.c tmp-SYSCALLS.s

test-protoize-simple: ./protoize ./unprotoize $(GCC_PASSES)
	-rm -f tmp-proto.[cso]
	cp $(srcdir)/protoize.c tmp-proto.c
	chmod u+w tmp-proto.c
	./protoize -N -B ./ -x getopt.h -c "-B./ -Wall -Wwrite-strings \
	  $(CFLAGS) $(INCLUDES) \
	  -DGCC_INCLUDE_DIR=0 \
	  -DGPLUSPLUS_INCLUDE_DIR=0 \
	  -DCROSS_INCLUDE_DIR=0 \
	  -DSTD_PROTO_DIR=0" tmp-proto.c
	@echo '**********' Expect 400 lines of differences.
	-diff $(srcdir)/protoize.c tmp-proto.c > tmp-proto.diff
	-wc -l tmp-proto.diff
	./unprotoize -N -x getopt.h -c "-B./ -Wall -Wwrite-strings \
	  $(CFLAGS) $(INCLUDES) \
	  -DGCC_INCLUDE_DIR=0 \
	  -DGPLUSPLUS_INCLUDE_DIR=0 \
	  -DCROSS_INCLUDE_DIR=0 \
	  -DSTD_PROTO_DIR=0" tmp-proto.c
	@echo Expect zero differences.
	diff $(srcdir)/protoize.c tmp-proto.c | cat
	-rm -f tmp-proto.[cso]

# Remake the info files.

doc: $(srcdir)/cpp.info $(srcdir)/gcc.info $(srcdir)/INSTALL

$(srcdir)/cpp.info: cpp.texi
	makeinfo `echo $(srcdir)/cpp.texi | sed 's,^\./,,'`

#$(srcdir)/gplus.info: gplus.texi
#	makeinfo `echo $(srcdir)/gplus.texi | sed 's,^\./,,'`

$(srcdir)/gcc.info: gcc.texi extend.texi install.texi invoke.texi \
		md.texi rtl.texi tm.texi 
	makeinfo `echo $(srcdir)/gcc.texi | sed 's,^\./,,'`

# This works with GNU Make's default rule.
$(srcdir)/gcc.dvi: gcc.texi extend.texi install.texi invoke.texi \
		md.texi rtl.texi tm.texi 

# This works with GNU Make's default rule.
$(srcdir)/cpp.dvi: cpp.texi

$(srcdir)/INSTALL: install1.texi install.texi
	makeinfo -D INSTALLONLY --no-header `echo $(srcdir)/install1.texi | sed 's,^\./,,'`

# Deletion of files made during compilation.
# There are four levels of this:
#   `mostlyclean', `clean', `distclean' and `realclean'.
# `mostlyclean' is useful while working on a particular type of machine.
# It deletes most, but not all, of the files made by compilation.
# It does not delete libgcc.a or its parts, so it won't have to be recompiled.
# `clean' deletes everything made by running `make all'.
# `distclean' also deletes the files made by config.
# `realclean' also deletes everything that could be regenerated automatically.


mostlyclean:
	-rm -f $(STAGESTUFF)
# Clean the objc subdir if we created one.
	if [ -d objc ]; then \
	  srcdir1=`cd $(srcdir); pwd`; \
	  cd objc; $(MAKE) -f $$srcdir1/objc/Makefile mostlyclean; \
	else true; fi
	-rm -f libobjc.a
# Delete the temporary source copies for cross compilation.
	-rm -f $(HOST_PREFIX_1)rtl.c $(HOST_PREFIX_1)rtlanal.c
	-rm -f $(HOST_PREFIX_1)alloca.c $(HOST_PREFIX_1)malloc.c
	-rm -f $(HOST_PREFIX_1)obstack.c 
# Delete the temp files made in the course of building libgcc.a.
	-rm -f tmplibgcc* tmpcopy xlimits.h
	for name in $(LIB1FUNCS); do rm -f $${name}.c; done
# Delete other temporary files.
	-rm -f tmp-float.h tmp-gcc.xtar.Z
	-rm -f tmp-foo1 tmp-foo2 tmp-proto.* tmp-unproto.1 tmp-SYSCALLS.s
# Delete the stamp files.
	-rm -f stamp-* tmp-*
# Delete debugging dump files.
	-rm -f *.greg *.lreg *.combine *.flow *.cse *.jump *.rtl *.tree *.loop
	-rm -f *.dbr *.jump2 *.sched *.cse2 *.sched2 *.stack
# Delete some files made during installation.
	-rm -f specs float.h enquire SYSCALLS.c.X SYSCALLS.c
	-rm -f collect collect2 ld mips-tfile mips-tdump alloca.s
# Delete unwanted output files from TeX.
	-rm -f *.toc *.log *.vr *.fn *.cp *.tp *.ky *.pg
# Delete sorted indices we don't actually use.
	-rm -f gcc.vrs gcc.kys gcc.tps gcc.pgs gcc.fns
# Delete core dumps.
	-rm -f core config/core

# Delete all files made by compilation
# that don't exist in the distribution.
clean: mostlyclean
# It may not be quite desirable to delete unprotoize.c here,
# but the spec for `make clean' requires it.
# Using unprotoize.c is not quite right in the first place, 
# but what better way is there?
	-rm -f libgcc.a libgcc1.a libgcc2.a libgcc2.ready libgcc1.null
	-rm -f *.dvi

# Delete all files that users would normally create
# while building and installing GCC.
distclean: clean
	-rm -f tm.h aux-output.c config.h md config.status tconfig.h hconfig.h
	-rm -f Makefile *.oaux
	-rm -fr stage1 stage2 stage3 stage4

# Delete anything likely to be found in the source directory
# that shouldn't be in the distribution.
extraclean: distclean
	-rm -rf =* #* *~* config/=* config/#* config/*~*
	-rm -f patch* *.orig *.rej config/patch* config/*.orig config/*.rej
	-rm -f *.dvi *.oaux *.d *.Z *.tar *.xtar *diff
	-rm -f *lose config/*lose
	-rm -f *.s *.s[0-9] *.i install1.texi config/ChangeLog

# Get rid of every file that's generated from some other file.
# Most of these files ARE PRESENT in the GCC distribution.
realclean: distclean
	-rm -f c-parse.y objc-parse.y
	-rm -f cp-parse.c cp-parse.h cp-parse.output
	-rm -f objc-parse.c objc-parse.output
	-rm -f c-parse.c c-parse.h c-parse.output
	-rm -f cexp.c cexp.output TAGS 
	-rm -f cpp.info* cpp.??s cpp.*aux
	-rm -f gcc.info* gcc.??s gcc.*aux
	-rm -f gplus.info* gplus.??s gplus.*aux

# Entry points `install' and `uninstall'.
# Also temporarily `install-fixincludes' could replace `install-headers'.
# Also use `install-collect2' to install collect2 when the config files don't.

# The semicolon is to prevent the install.sh -> install default rule
# from doing anything.
install: $(INSTALL_TARGET) ;

# Copy the files of native compiler into directories where they will be run.
install-native: install-common install-man install-libgcc

# Copy the files of cross compiler into directories where they will be run.
install-cross: install-common install-common-headers \
   install-man install-cross-tools install-libgcc

# Do nothing while making gcc with a cross-compiler. The person who
# makes gcc for the target machine has to know how to put a complete
# gcc together by hand.
install-build: force
	@echo You have to install gcc on your target machine by hand.

# Install the tools, libraries and header files for the target machine
# where cross-compilation will look for them.
# Use tooldir to find them.
install-cross-tools: install-dir
# The first if makes this a no-op except for a cross compiler.
# The /. after the dirname causes test to follow symlinks.
# Before making a link or an indirection script, 
# we verify the desired file does not already exist.
# If a symlink does exist, then making a symlink would certainly fail, 
# leading us to overwrite the real file through the symlink.
	-if [ -f gcc-cross ] ; \
	then \
	  if [ -d $(tooldir)/. ] ; \
	  then \
	    for file in as ld ar nm ranlib; do \
	      if [ -f $(libsubdir)/$$file ] ; \
	      then true; \
	      else  \
		rm -rf $(libsubdir)/$$file; \
		$(SYMLINK) $(tooldir)/bin/$$file $(libsubdir)/$$file \
		|| (echo "#!/bin/sh"; echo $(tooldir)/bin/$$file "$$@") > $(libsubdir)/$$file; \
	      fi; \
	    done; \
	    for file in $(tooldir)/lib/*; do \
	      if [ -f $$file ] ; \
	      then \
		if [ -f $(libsubdir)/`basename $$file` ] ; \
		then true; \
		else  \
		  rm -rf $(libsubdir)/`basename $$file`; \
		  $(SYMLINK) $$file $(libsubdir)/`basename $$file` \
		  || $(INSTALL_DATA) $$file $(libsubdir)/`basename $$file`; \
		fi; \
	      else true; \
	      fi; \
	    done; \
	    if [ -d $(tooldir)/include/. ] ; then \
	      rm -rf $(libsubdir)/sys-include; \
	      $(SYMLINK) $(tooldir)/include $(libsubdir)/sys-include \
	      || (if [ -d $(libsubdir)/sys-include ] ; then true ; else mkdir $(libsubdir)/sys-include ; fi; \
		  (cd $(tooldir)/include; tar cf - .) | (cd $(libsubdir)/sys-include; tar xpf -)); \
	    else true; fi; \
	  else true; \
	  fi; \
	else true; \
	fi;

# Run this on the target machine
# to finish installation of cross compiler.
install-cross-rest: install-float-h-cross

# Install float.h for cross compiler.
# Run this on the target machine!
install-float-h-cross:
# I don't see what this line is for.
# If you see what good it does, please tell me.  -- rms.
	if [ -f enquire ] ; then true; else false; fi
	-./enquire -f > $(tmpdir)/float.h
	-rm -f $(libsubdir)/include/float.h
	$(INSTALL_DATA) $(tmpdir)/float.h $(libsubdir)/include/float.h
	-rm -f $(tmpdir)/float.h
	chmod a-x $(libsubdir)/include/float.h

# Create the installation directory.
install-dir:
	-if [ -d $(libdir) ] ; then true ; else mkdir $(libdir) ; fi
D 2
	-if [ -d $(libdir)/gcc-lib ] ; then true ; else mkdir $(libdir)/gcc-lib ; fi
	-if [ -d $(libdir)/gcc-lib/include ] ; then true ; else mkdir $(libdir)/gcc-lib/include ; fi
	-if [ -d $(libdir)/gcc-lib/$(target) ] ; then true ; else mkdir $(libdir)/gcc-lib/$(target) ; fi
	-if [ -d $(libdir)/gcc-lib/$(target)/$(version) ] ; then true ; else mkdir $(libdir)/gcc-lib/$(target)/$(version) ; fi
	-if [ -d $(libdir)/gcc-lib/$(target)/$(version)/include ] ; then true ; else mkdir $(libdir)/gcc-lib/$(target)/$(version)/include ; fi
	-if [ -d $(bindir) ] ; then true ; else mkdir $(bindir) ; fi
	-if [ -d $(includedir) ] ; then true ; else mkdir $(includedir) ; fi
	-if [ -d $(includedir) ] ; then true ; else mkdir $(includedir) ; fi
	-if [ -f gcc-cross ] ; \
	then \
	  if [ -d $(tooldir) ] ; then true ; else mkdir $(tooldir) ; fi ; \
	  if [ -d $(assertdir) ] ; then true ; else mkdir $(assertdir) ; fi ; \
	else true; \
	fi
# We don't use mkdir -p to create the parents of mandir,
# because some systems don't support it.
# Instead, we use this technique to create the immediate parent of mandir.
	-parent=`echo $(mandir)|sed -e 's@/[^/]*$$@@'`; \
	if [ -d $$parent ] ; then true ; else mkdir $$parent ; fi
	-if [ -d $(mandir) ] ; then true ; else mkdir $(mandir) ; fi
E 2

# Install the compiler executables built during cross compilation.
# Deps on  $(srcdir)/g++ $(srcdir)/c++  would be natural here,
# but the latter would get confused with the target `c++'.
#install-common: native install-dir xgcc $(EXTRA_PARTS)
install-common: install-dir xgcc $(EXTRA_PARTS)
	for file in $(COMPILERS); do \
	  if [ -f $$file ] ; then \
	    rm -f $(libsubdir)/$$file; \
	    $(INSTALL_PROGRAM) $$file $(libsubdir)/$$file; \
	  else true; \
	  fi; \
	done
	for file in $(EXTRA_PASSES) $(EXTRA_PARTS) $(EXTRA_PROGRAMS) $(USE_COLLECT2) ..; do \
	  if [ x"$$file" != x.. ]; then \
	    rm -f $(libsubdir)/$$file; \
	    $(INSTALL_PROGRAM) $$file $(libsubdir)/$$file; \
	  else true; fi; \
	done
# Don't mess with specs if it doesn't exist yet.
	-if [ -f specs ] ; then \
	  rm -f $(libsubdir)/specs; \
	  $(INSTALL_DATA) specs $(libsubdir)/specs; \
	fi
# Install the driver program as gcc-$(target)
# and also as either gcc (if native) or $(tooldir)/bin/gcc.
	-if [ -f gcc-cross ] ; then \
	  $(INSTALL_PROGRAM) gcc-cross $(bindir)/gcc-$(target); \
	  if [ -d $(tooldir)/bin/. ] ; then \
	    rm -f $(tooldir)/bin/gcc; \
	    $(INSTALL_PROGRAM) gcc-cross $(tooldir)/bin/gcc; \
	  else true; fi; \
	else \
	  rm -f $(bindir)/gcc; \
	  $(INSTALL_PROGRAM) xgcc $(bindir)/gcc-2.3.3; \
	  ln -s /usr/bin/gcc $(bindir)/gcc-2.3.3 \
	  rm -f $(bindir)/gcc-$(target)-1; \
	  ln $(bindir)/gcc-2.3.3 $(bindir)/gcc-$(target)-1; \
	  mv $(bindir)/gcc-$(target)-1 $(bindir)/gcc-$(target); \
	fi
# Install protoize if it was compiled.
	-if [ -f protoize ]; \
	then \
	    rm -f $(bindir)/protoize-2.3.3; \
	    $(INSTALL_PROGRAM) protoize $(bindir)/protoize-2.3.3; \
	    rm -f $(bindir)/unprotoize; \
	    $(INSTALL_PROGRAM) unprotoize $(bindir)/unprotoize-2.3.3; \
	    rm -f $(libsubdir)/SYSCALLS.c.X; \
	    $(INSTALL_DATA) SYSCALLS.c.X $(libsubdir)/SYSCALLS.c.X; \
	    -chmod a-x $(libsubdir)/SYSCALLS.c.X; \
	fi
	-rm -f $(bindir)/c++-2.3.3
	$(INSTALL_PROGRAM) $(srcdir)/c++ $(bindir)/c++-2.3.3
	-rm -f $(bindir)/g++-2.3.3
	$(INSTALL_PROGRAM) $(srcdir)/g++ $(bindir)/g++-2.3.3
	-rm -f $(libsubdir)/cpp
	$(INSTALL_PROGRAM) cpp $(libsubdir)/cpp

# Install the man pages.
install-man: install-dir $(srcdir)/gcc.1 $(srcdir)/cccp.1 $(srcdir)/g++.1
	-rm -f $(mandir)/gcc$(manext)
	$(GROFF) gcc.1 > gcc.0
	$(GROFF) cccp.1 > cccp.0
	$(GROFF) g++.1 > g++.0
	-$(INSTALL_DATA) $(srcdir)/gcc.0 $(mandir)/gcc$(manext)
	-chmod a-x $(mandir)/gcc$(manext)
	-rm -f $(mandir)/cccp$(manext) $(mandir)/cpp$(manext)
	-$(INSTALL_DATA) $(srcdir)/cccp.0 $(mandir)/cccp$(manext)
	-chmod a-x $(mandir)/cccp$(manext)
	ln -s $(mandir)/cccp$(manext) $(mandir)/cpp$(manext)
	-$(INSTALL_DATA) $(srcdir)/g++.0 $(mandir)/g++$(manext)
	-chmod a-x $(mandir)/g++$(manext)

# Install the library.
#install-libgcc: libgcc.a install-dir
install-libgcc: install-dir
	-if [ -f libgcc.a ] ; then \
	  rm -f $(libsubdir)/libgcc.a; \
	  $(INSTALL_DATA) libgcc.a $(libsubdir)/libgcc.a; \
	  if $(RANLIB_TEST) ; then \
	    (cd $(libsubdir); $(RANLIB) libgcc.a); else true; fi; \
	  chmod a-x $(libsubdir)/libgcc.a; \
	else true; fi

# Install the objc run time library.
install-libobjc: libobjc.a install-dir
	-if [ -f libobjc.a ] ; then \
	  rm -f $(libsubdir)/libobjc.a; \
	  $(INSTALL_DATA) libobjc.a $(libsubdir)/libobjc.a; \
	  if $(RANLIB_TEST) ; then \
	    (cd $(libsubdir); $(RANLIB) libobjc.a); else true; fi; \
	  chmod a-x $(libsubdir)/libobjc.a; \
	else true; fi

# Install all the header files for native compiler.
install-headers: install-common-headers install-float-h install-limits-h

# Install float.h for native compiler.
install-float-h: float.h install-dir
	-rm -f $(libsubdir)/include/float.h
	$(INSTALL_DATA) float.h $(libsubdir)/include/float.h
	-chmod a-x $(libsubdir)/include/float.h

# Install limits.h.
install-limits-h: xlimits.h install-dir
	-rm -f $(libsubdir)/include/limits.h
	$(INSTALL_DATA) xlimits.h $(libsubdir)/include/limits.h
	-chmod a-x $(libsubdir)/include/limits.h

# Install the fixed headers that are the same for all machines.
install-common-headers: install-dir $(USER_H) gvarargs.h gstdarg.h gstddef.h gsyslimits.h assert.h
	-if [ -d $(libsubdir)/include ] ; then true ; else mkdir $(libsubdir)/include ; fi
	-chmod ugo+rx $(libsubdir)/include
# Must compute $(libsubdir) before the cd; the awk script won't work after.
	-shelllibsubdir=$(libsubdir); \
	cd $(srcdir); \
	for file in $(USER_H); do \
	   rm -f $$shelllibsubdir/include/`basename $$file`; \
	   $(INSTALL_DATA) `basename $$file` $$shelllibsubdir/include/`basename $$file`; \
	   chmod a-x $$shelllibsubdir/include/`basename $$file`; \
	done
# Put assert.h in /usr/local/include, so it won't override GNU libc's assert.h.
# Don't replace the assert.h already there if it is not from GCC.
# This code would be simpler if it tested for -f ... && ! grep ...
# but supposedly the ! operator is missing in sh on some systems.
	-if [ -f $(assertdir)/assert.h ]; \
	then \
	  if grep "__eprintf" $(assertdir)/assert.h; \
	    then \
	    rm -f $(assertdir)/assert.h; \
	    $(INSTALL_DATA) $(srcdir)/assert.h $(assertdir)/assert.h; \
	    chmod a-x $(assertdir)/assert.h; \
	  else true; \
	  fi; \
	else \
	  rm -f $(assertdir)/assert.h; \
	  $(INSTALL_DATA) $(srcdir)/assert.h $(assertdir)/assert.h; \
	  chmod a-x $(assertdir)/assert.h; \
	fi
	-rm -f $(libsubdir)/include/syslimits.h
	$(INSTALL_DATA) $(srcdir)/gsyslimits.h $(libsubdir)/include/syslimits.h
	-chmod a-x $(libsubdir)/include/syslimits.h
	-rm -f $(libsubdir)/include/varargs.h
	$(INSTALL_DATA) $(srcdir)/gvarargs.h $(libsubdir)/include/varargs.h
	-chmod a-x $(libsubdir)/include/varargs.h
	-rm -f $(libsubdir)/include/stdarg.h
	$(INSTALL_DATA) $(srcdir)/gstdarg.h $(libsubdir)/include/stdarg.h
	-chmod a-x $(libsubdir)/include/stdarg.h
	-rm -f $(libsubdir)/include/stddef.h
	$(INSTALL_DATA) $(srcdir)/gstddef.h $(libsubdir)/include/stddef.h
	-chmod a-x $(libsubdir)/include/stddef.h
# This is turned off because fixinc.svr4 can now get it directly from srcdir.
## Copy byteorder.h into the object file directory 
## so that fixinc.svr4 can get at it if necessary.
## If the dirs are the same, this won't do anything.
## Delete file first in case it is read-only
#	-if [ x`cd $(srcdir);pwd` != x`pwd` ]; then rm -f byteorder.h; else true; fi
#	-cp $(srcdir)/byteorder.h . > /dev/null 2>&1

# $(libsubdir)/include:
#	-if [ -d $(libsubdir)/include ] ; then true ; else mkdir $(libsubdir)/include ; fi
#	-chmod ugo+rx $(libsubdir)/include

# This appears not to work.  It isn't clear how to fix it.
# $(libsubdir)/include/README: $(libsubdir)/include $(srcdir)/$(FIXINCLUDES)
#	LIB=$(libsubdir)/include $(srcdir)/$(FIXINCLUDES)
#	$(INSTALL_DATA) $(srcdir)/fixincludes-README $@
#	chmod a-x $@

# Run fixincludes in the proper directory.
install-fixincludes: install-headers
	rm -rf $(libsubdir)/tmp
	mkdir $(libsubdir)/tmp
# Move aside the headers that come from GCC; delete all else.
# The sed command gets just the last file name component;
# this is necessary because VPATH could add a dirname.
# Using basename would be simpler, but some systems don't have it.
	cd $(libsubdir)/include; \
	for file in $(INSTALLED_H); do \
	  realfile=`echo $$file | sed -e 's|.*/\([^/]*\)$$|\1|'`; \
	  mv $$realfile ../tmp; \
	done; \
	rm -rf *
# Install fixed copies of system files.
	for dir in $(SYSTEM_HEADER_DIR) $(OTHER_FIXINCLUDES_DIRS); do \
	  if [ -d $$dir ]; \
	  then \
	    $(srcdir)/$(FIXINCLUDES) $(libsubdir)/include $$dir $(srcdir); \
	  else true; fi; \
	done
	-cd $(libsubdir)/include; \
	if [ -f limits.h ]; then \
	  rm -f ../tmp/syslimits.h; \
	  cp limits.h ../tmp/syslimits.h; \
	else true; fi
# Bring back gcc's header files.
	cd $(libsubdir)/include; mv ../tmp/* .; rmdir ../tmp
# Install the README
	$(INSTALL_DATA) $(srcdir)/README-fixinc $(libsubdir)/include/README
	-chmod a-x $(libsubdir)/include/README

# Use this target to install the program `collect2' under the name `ld'.
install-collect2: collect2
	$(INSTALL_PROGRAM) collect2 $(libsubdir)/ld
# Install the driver program as $(libsubdir)/gcc for collect2.
	$(INSTALL_PROGRAM) xgcc $(libsubdir)/gcc

# Cancel installation by deleting the installed files.
uninstall:
	-rm -rf $(libsubdir)
	-rm -rf $(bindir)/gcc
	-rm -rf $(bindir)/protoize
	-rm -rf $(bindir)/unprotoize
	-rm -rf $(mandir)/gcc$(manext)
	-rm -rf $(mandir)/cccp$(manext)
	-rm -rf $(mandir)/protoize$(manext)
	-rm -rf $(mandir)/unprotoize$(manext)

# These exist for maintenance purposes.

# Update the tags table.
TAGS: force
	cd $(srcdir);							\
	mkdir temp;							\
	mv -f c-parse.[ch] cp-parse.[ch] objc-parse.c cexp.c =*.[chy] temp; \
	etags *.y *.h *.c;						\
	mv temp/* .;							\
	rmdir temp

# Create the distribution tar file.
#dist: gcc-$(version).tar.Z
dist: gcc.xtar.Z

gcc.xtar.Z: gcc.xtar
	compress < gcc.xtar > tmp-gcc.xtar.Z
	mv tmp-gcc.xtar.Z gcc.xtar.Z

#gcc-$(version).tar.Z: gcc-$(version).tar
#	compress < gcc-$(version).tar > gcc-$(version).tar.Z

#gcc-$(version).tar:
gcc.xtar: doc c-parse.y objc-parse.y
	if grep -s "for version ${version}" gcc.texi; \
	then true; \
	else echo "You must update the version number in \`gcc.texi'"; sleep 10;\
	fi
# Update the version number in README
	awk '$$1 " " $$2 " " $$3 == "This directory contains" \
		{ $$6 = version; print $$0 } \
	     $$1 " " $$2 " " $$3 != "This directory contains"' \
	  version=$(version) README > tmp.README
	mv tmp.README README
	-rm -rf gcc-$(version) tmp	
# Put all the files in a temporary subdirectory
# which has the name that we want to have in the tar file.
	mkdir tmp
	mkdir tmp/config
	mkdir tmp/objc
	for file in *[0-9a-zA-Z+]; do \
	  ln $$file tmp > /dev/null 2>&1 || cp $$file tmp; \
	done
	cd config; \
	for file in *[0-9a-zA-Z+]; do \
	  ln $$file ../tmp/config >/dev/null 2>&1 || cp $$file ../tmp/config; \
	done
	cd objc; \
	for file in *[0-9a-zA-Z+]; do \
	  ln $$file ../tmp/objc >/dev/null 2>&1 || cp $$file ../tmp/objc; \
	done
	ln .gdbinit tmp
	mv tmp gcc-$(version)
# Get rid of everything we don't want in the distribution.
	cd gcc-$(version); make -f Makefile.in extraclean
# Make the distribution.
	tar chf gcc.xtar gcc-$(version)
# Get rid of the temporary directory.
	rm -rf gcc-$(version)

# do make -f ../gcc/Makefile maketest DIR=../gcc
# in the intended test directory to make it a suitable test directory.
# THIS IS OBSOLETE; use the -srcdir operand in configure instead. 
maketest:
	ln -s $(DIR)/*.[chy] .
	ln -s $(DIR)/configure .
	ln -s $(DIR)/*.def .
	-rm -f =*
	ln -s $(DIR)/.gdbinit .
	ln -s $(DIR)/$(FIXINCLUDES) .
	-ln -s $(DIR)/bison.simple .
	ln -s $(DIR)/config .
	ln -s $(DIR)/move-if-change .
# The then and else were swapped to avoid a problem on Ultrix.
	if [ ! -f Makefile ] ; then ln -s $(DIR)/Makefile .; else false; fi
	-rm tm.h aux-output.c config.h md
	make clean
# You must then run config to set up for compilation.

bootstrap: force
# Only build the C compiler for stage1, because that is the only one that
# we can guarantee will build with the native compiler, and also it is the
# only thing useful for building stage2.
	$(MAKE) LANGUAGES=c
	$(MAKE) stage1
# This used to define ALLOCA as empty, but that would lead to bad results
# for a subsequent `make install' since that would not have ALLOCA empty.
# To prevent `make install' from compiling alloca.o and then relinking cc1
# because alloca.o is newer, we permit these recursive makes to compile
# alloca.o.  Then cc1 is newer, so it won't have to be relinked.
	$(MAKE) CC="stage1/xgcc -Bstage1/" CFLAGS="$(BOOT_CFLAGS)" libdir=$(libdir) LANGUAGES="$(LANGUAGES)"
	$(MAKE) stage2
	$(MAKE) CC="stage2/xgcc -Bstage2/" CFLAGS="$(BOOT_CFLAGS)" libdir=$(libdir) LANGUAGES="$(LANGUAGES)"

bootstrap2: force
	$(MAKE) CC="stage1/xgcc -Bstage1/" CFLAGS="$(BOOT_CFLAGS)" libdir=$(libdir) LANGUAGES="$(LANGUAGES)"
	$(MAKE) stage2
	$(MAKE) CC="stage2/xgcc -Bstage2/" CFLAGS="$(BOOT_CFLAGS)" libdir=$(libdir) LANGUAGES="$(LANGUAGES)"

bootstrap3: force
	$(MAKE) CC="stage2/xgcc -Bstage2/" CFLAGS="$(BOOT_CFLAGS)" libdir=$(libdir) LANGUAGES="$(LANGUAGES)"

# Compare the object files in the current directory with those in the
# stage2 directory.

compare: force
	for file in *.o; do \
	  tail +16c $$file > tmp-foo1; \
	  tail +16c stage2/$$file > tmp-foo2 2>/dev/null \
	    && (cmp tmp-foo1 tmp-foo2 || echo $$file differs); \
	done
	-rm -f tmp-foo*

# Similar, but compare with stage3 directory
compare3: force
	for file in *.o; do \
	  tail +16c $$file > tmp-foo1; \
	  tail +16c stage3/$$file > tmp-foo2 2>/dev/null \
	    && (cmp tmp-foo1 tmp-foo2 || echo $$file differs); \
	done
	-rm -f tmp-foo*

# Copy the object files from a particular stage into a subdirectory.
stage1: force
	-if [ -d stage1 ] ; then true ; else mkdir stage1 ; fi
	-mv $(STAGESTUFF) stage1
	-rm -f stage1/libgcc.a
	-cp libgcc.a stage1
	-if $(RANLIB_TEST) ; then $(RANLIB) stage1/libgcc.a; else true; fi

stage2: force
	-if [ -d stage2 ] ; then true ; else mkdir stage2 ; fi
	-mv $(STAGESTUFF) stage2
	-rm -f stage2/libgcc.a
	-cp libgcc.a stage2
	-if $(RANLIB_TEST) ; then $(RANLIB) stage2/libgcc.a; else true; fi

stage3: force
	-if [ -d stage3 ] ; then true ; else mkdir stage3 ; fi
	-mv $(STAGESTUFF) stage3
	-rm -f stage3/libgcc.a
	-cp libgcc.a stage3
	-if $(RANLIB_TEST) ; then $(RANLIB) stage3/libgcc.a; else true; fi

stage4: force
	-if [ -d stage4 ] ; then true ; else mkdir stage4 ; fi
	-mv $(STAGESTUFF) stage4
	-rm -f stage4/libgcc.a
	-cp libgcc.a stage4
	-if $(RANLIB_TEST) ; then $(RANLIB) stage4/libgcc.a; else true; fi

# Copy just the executable files from a particular stage into a subdirectory,
# and delete the object files.  Use this if you're just verifying a version
# that is pretty sure to work, and you are short of disk space.
risky-stage1: force
	-if [ -d stage1 ] ; then true ; else mkdir stage1 ; fi
	-mv cc1 cpp cccp gcc stage1
	-rm -f stage1/libgcc.a
	-cp libgcc.a stage1 && $(RANLIB) stage1/libgcc.a
	-make clean

risky-stage2: force
	-if [ -d stage2 ] ; then true ; else mkdir stage2 ; fi
	-mv cc1 cpp cccp gcc stage2
	-rm -f stage2/libgcc.a
	-cp libgcc.a stage2 && $(RANLIB) stage2/libgcc.a
	-make clean

risky-stage3: force
	-if [ -d stage3 ] ; then true ; else mkdir stage3 ; fi
	-mv cc1 cpp cccp gcc stage3
	-rm -f stage3/libgcc.a
	-cp libgcc.a stage3 && $(RANLIB) stage3/libgcc.a
	-make clean

risky-stage4: force
	-if [ -d stage4 ] ; then true ; else mkdir stage4 ; fi
	-mv cc1 cpp cccp gcc stage4
	-rm -f stage4/libgcc.a
	-cp libgcc.a stage4 && $(RANLIB) stage4/libgcc.a
	-make clean

#In GNU Make, ignore whether `stage*' exists.
.PHONY: stage1 stage2 stage3 stage4 clean realclean TAGS bootstrap
.PHONY: risky-stage1 risky-stage2 risky-stage3 risky-stage4

force:
E 1
