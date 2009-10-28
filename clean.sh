#!/bin/bash -u
export WaitTime=0
cd clip-prg.32-64
[ -f Makefile ] || ./configure "local"
make clean
[ -f Makefile ] && make clean
rm -fv Makefile clean
