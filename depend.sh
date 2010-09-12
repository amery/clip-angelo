#!/bin/bash -u
export WaitTime=1
init/compile.sh "depend"
if [[ $? != 0 ]] ; then
	rm -fv ./depend
	exit 1
fi
if [ -f ./depend ] ; then
	rm -fv ./depend
fi
