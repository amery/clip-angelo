#!/bin/bash -u
export WaitTime=1
init/compile.sh "local"
if [[ $? != 0 ]] ; then
	rm -fv ./local
	exit 1
fi
if [ -f ./local ] ; then
	rm -fv ./local
fi
