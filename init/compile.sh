#!/bin/sh -u

export Clip_M_Dir="$PWD"
makecmdgoals=$1
rm -fv compile*.$makecmdgoals.sh
exec 1>>compile.$makecmdgoals.sh
exec 2>>compile2.$makecmdgoals.sh
if ! [ -f Makefile ] ; then
	./configure "$makecmdgoals"
	cd "$Clip_M_Dir"
fi
make "$makecmdgoals"
if [[ $? != 0 ]] ; then
	banner "$makecmdgoals error(s)" 							>&0
	banner "$makecmdgoals error(s)" 							>&1
	banner "$makecmdgoals error(s)" 							>&2
fi
echo "Log file : $PWD/compile*.*.sh" 						>&0
