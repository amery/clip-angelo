#!/bin/bash -ux
export Clip_M_Dir="$PWD"
if ! [ -f Makefile ] ; then
	./configure "opt"
	cd "$Clip_M_Dir"
fi
make "opt"
