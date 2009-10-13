#!/bin/bash -ux
init/compile.sh "opt"
if [[ $? != 0 ]] ; then
	init/Make.clean.sh
fi

[ -f ./opt ] && rm -fv ./opt
