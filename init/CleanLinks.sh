#!/bin/bash -u
#
# erases linked symbol files
#

echo "removing link symbols" >&0
if [ -z $1 ] ; then
	play sounds/k3b_error1.wav &
	sleep $SleepTime
	exit 1
fi
function CleanLinks ()
{
    local Cur_File
	cd $1
	echo scanning $PWD
	echo Removing links >/dev/null
	for Cur_File in $(ls -A) ; do
		if [ -L $1/$Cur_File ] ; then
			echo removed $1/$Cur_File >/dev/null
			rm -f$V $1/$Cur_File
			rm -f$V $1/compile.*.sh
		elif [ -d $Cur_File ] ; then
			CleanLinks $1/$Cur_File
		fi
#		sleep $SleepTime
	done
	cd ..
}
CleanLinks $1
