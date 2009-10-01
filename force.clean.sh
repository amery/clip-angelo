#!/bin/sh

rm -fv Makefile
rm -fv */Makefile
rm -fv */*/Makefile
rm -fv */*/*/Makefile
rm -fv */*/*/*/Makefile
rm -fv */*/*/*/*/Makefile
rm -fv */*/*/*/*/*/Makefile
rm -fv */*/*/*/*/*/*/Makefile
rm -fv */*/*/*/*/*/*/configure
rm -fv */*/*/*/*/*/configure
rm -fv */*/*/*/*/configure
rm -fv */*/*/*/configure
rm -fv */*/*/configure
rm -fv */*/configure
rm -fv */configure
./clean.sh
