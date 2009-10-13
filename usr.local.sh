#!/bin/bash -u
init/compile.sh "usr.local"
if [[ $? != 0 ]] ; then
	init/Make.clean.sh
fi

[ -f ./usr.local ] && rm -fv ./usr.local
