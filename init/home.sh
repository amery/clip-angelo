#!/bin/bash -u
export WaitTime=1
init/compile.sh "home"
if [[ $? != 0 ]] ; then
	rm -fv ./home
	exit 1
fi
if [ -f ./home ] ; then
	rm -fv ./home
fi
