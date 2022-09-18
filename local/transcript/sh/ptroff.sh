#! /bin/sh
# sh/ptroff.bsd
# Copyright (c) 1985 Adobe Systems Incorporated
# RCSID: $Header: ptroff.bsd,v 2.1 85/11/24 12:32:28 shore Rel $
#
# run troff in an environment to print on a PostScript printer
#
# ptroff - troff | pscat [| lpr]

opt= spool= 
psfontlib=TROFFFONTDIR
font=-F${psfontlib}
family=Times
printer=-P${PRINTER-PostScript}
while test "$1" != ""
do	case "$1" in
	-F)	if test "$#" -lt 2 ; then
			echo '-F takes following font family name' 1>&2
			exit 1 
		fi
		family=$2 ; shift ;;
	-F*)	echo 'use -F familyname' 1>&2 ;
		exit 1 ;;
	-t)	nospool=1 ;;
	-#*|-h|-m)	spool="$spool $1" ;;
	-P*)	printer=$1 ;;
	-C)	spool="$spool $1 $2"
		classname=$2 ; shift ;;
	-J)	jobname=$2 ; shift ;;

	-)	fil="$fil $1" ;;
	-*)	opt="$opt $1" ;;

	*)	fil="$fil $1" ; jobname=${jobname-$1} ;;
	esac
	shift
done
spool="$printer $spool"
if test "$jobname" = "" ; then
	jobname="Troff"
fi
spool="-J $jobname $spool"
if test "$fil" = "" ; then
	fil="-"
fi
troff="troff -F${psfontlib}/ftXX -t $opt ${psfontlib}/${family}.head $fil "
pscat="pscat -F${psfontlib}/${family}.ct "

if test "$nospool" = "1" ; then
	$troff | $pscat
else
	$troff | $pscat | lpr $spool
fi
