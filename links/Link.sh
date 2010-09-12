#!/bin/bash -u

source $Clip_M_Dir/init/functions.f.sh

OnAllScreen $PWD/$0

if [[ "$C64" = "64" ]] ; then
	cd 64
	EXE="64"
else
	cd 32
	EXE="32"
fi
export EXE
./Link.sh
if [[ $? != 0 ]] ; then
	error $0 $LINENO
	exit 1
fi
