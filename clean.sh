#!/bin/bash -u
cd clip-prg.32-64
[ -f Makefile ] || ./configure "home"
make clean
[ -f Makefile ] && make clean
rm -fv Makefile
