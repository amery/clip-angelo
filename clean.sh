#!/bin/bash -u
cd clip-prg.32-64
[ -f Makefile ] || ./configure "local"
make clean
[ -f Makefile ] && make clean
rm -fv Makefile clean
