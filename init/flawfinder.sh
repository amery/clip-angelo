#!/bin/bash -u
export WaitTime=1
init/compile.sh "flawfinder"
if [[ $? != 0 ]] ; then
	rm -fv ./flawfinder
	exit 1
fi
if [ -f ./flawfinder ] ; then
	rm -fv ./flawfinder
fi
