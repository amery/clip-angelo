#!/bin/bash -ux
cd clip-prg.32-64
[ -f Makefile ] || ./configure
make -i clean
[ -f Makefile ] && make clean
rm -fv Makefile