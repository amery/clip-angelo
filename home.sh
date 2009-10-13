#!/bin/bash -u
init/compile.sh "home"
if [[ $? != 0 ]] ; then
	init/Make.clean.sh
fi
[ -f ./home ] && rm -fv ./home
