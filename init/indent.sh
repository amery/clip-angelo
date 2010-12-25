#!/bin/bash -u

export Clip_M_Dir="$PWD"
[ -d temp ] || mkdir -pv temp
makecmdgoals=indent
rm -fv compile*.$makecmdgoals.sh
rm -fv temp/*
exec 1>>temp/compile.$makecmdgoals.sh
exec 2>>temp/compile2.$makecmdgoals.sh
if ! [ -f Makefile ] ; then
	./configure "$makecmdgoals"
	cd "$Clip_M_Dir"
fi
make "$makecmdgoals"
if [[ $? != 0 ]] ; then
	banner $makecmdgoals  							>&0
	banner "error(s)" 								>&0
	banner $makecmdgoals  							>&1
	banner "error(s)" 								>&1
	banner $makecmdgoals  							>&2
	banner "error(s)" 								>&2
	echo "Log file : $PWD/temp/compile*.*.sh" 				>&0
	exec 									1>&-
	exec 									2>&-
	exit 1
	exit 1
fi
