#!/bin/sh

rm -fv Makefile
rm -fv */Makefile
rm -fv */*/Makefile
rm -fv */*/*/Makefile
rm -fv */*/*/*/Makefile
rm -fv */*/*/*/*/Makefile
rm -fv */*/*/*/*/*/Makefile
rm -fv */*/*/*/*/*/*/Makefile
./clean.sh
