#!/bin/sh -ux
export WaitTime=0

rm -fv Makefile Makefile.ini configure.ini
rm -fv */Makefile */Makefile.ini */configure */configure.ini
rm -fv */*/Makefile */*/Makefile.ini */*/configure */*/*/configure.ini
rm -fv */*/*/Makefile */*/*/Makefile.ini */*/*/configure */*/*/configure.ini
rm -fv */*/*/*/Makefile */*/*/*/Makefile.ini */*/*/*/configure */*/*/*/configure.ini
rm -fv */*/*/*/*/Makefile */*/*/*/Makefile.ini */*/*/*/*/configure */*/*/*/*/configure.ini
rm -fv */*/*/*/*/*/Makefile */*/*/*/*/*/Makefile.ini */*/*/*/*/*/configure*/*/*/*/*/*/configure.ini
rm -fv */*/*/*/*/*/*/Makefile */*/*/*/*/*/*/Makefile.ini */*/*/*/*/*/*/configure */*/*/*/*/*/*/configure.ini
rm -fv *.ok
rm -fv */*.ok
rm -fv */*/*.ok
rm -fv */*/*/*.ok
rm -fv */*/*/*/*.ok
rm -fv */*/*/*/*/*.ok
rm -fv */*/*/*/*/*/*.ok
rm -fv */*/*/*/*/*/*/*.ok
rm -fv *.no
rm -fv */*.no
rm -fv */*/*.no
rm -fv */*/*/*.no
rm -fv */*/*/*/*.no
rm -fv */*/*/*/*/*.no
rm -fv */*/*/*/*/*/*.no
rm -fv */*/*/*/*/*/*/*.no
