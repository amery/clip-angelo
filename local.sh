#!/bin/bash -u
init/compile.sh "local"
if [[ $? != 0 ]] ; then
	init/Make.clean.sh
fi
[ -f ./local ] && rm -fv ./local
