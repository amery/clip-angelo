#!/bin/bash -u
source $Clip_M_Dir/init/functions.f.sh
export WaitTime=0
cd $Clip_M_Dir
[ -f Makefile ] || ./configure "local"
make clean
if [ -f Makefile ] ; then
	make clean
	if [[ $? != 0 ]] ; then
		rm -fv Makefile clean
		exit 1
	fi
fi
rm -fv Makefile clean
rm -fv Makefile clean configure.ok user || true
./init/force.clean.sh || true