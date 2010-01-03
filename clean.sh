#!/bin/bash -u
export WaitTime=0
cd clip-prg.32-64
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
