#!/bin/bash -ux
cd xDevil-prg.32-64
./[ -f Makefile ] || ./configure "home"
make distclean
rm -fv Makefile
