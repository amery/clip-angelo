#!/bin/sh -ux
export WaitTime=0

rm -fv Makefile Makefile.ini configure.ini 							|| true
rm -fv */Makefile */Makefile.ini */configure */configure.ini 							|| true
rm -fv */*/Makefile */*/Makefile.ini */*/configure */*/*/configure.ini 							|| true
rm -fv */*/*/Makefile */*/*/Makefile.ini */*/*/configure */*/*/configure.ini 							|| true
rm -fv */*/*/*/Makefile */*/*/*/Makefile.ini */*/*/*/configure */*/*/*/configure.ini 							|| true
rm -fv */*/*/*/*/Makefile */*/*/*/Makefile.ini */*/*/*/*/configure */*/*/*/*/configure.ini 							|| true
rm -fv */*/*/*/*/*/Makefile */*/*/*/*/*/Makefile.ini */*/*/*/*/*/configure*/*/*/*/*/*/configure.ini 							|| true
rm -fv */*/*/*/*/*/*/Makefile */*/*/*/*/*/*/Makefile.ini */*/*/*/*/*/*/configure */*/*/*/*/*/*/configure.ini 							|| true
rm -fv *.ok 							|| true
rm -fv */*.ok 							|| true
rm -fv */*/*.ok 							|| true
rm -fv */*/*/*.ok 							|| true
rm -fv */*/*/*/*.ok 							|| true
rm -fv */*/*/*/*/*.ok 							|| true
rm -fv */*/*/*/*/*/*.ok 							|| true
rm -fv */*/*/*/*/*/*/*.ok 							|| true
rm -fv *.no 							|| true
rm -fv */*.no 							|| true
rm -fv */*/*.no 							|| true
rm -fv */*/*/*.no 							|| true
rm -fv */*/*/*/*.no 							|| true
rm -fv */*/*/*/*/*.no 							|| true
rm -fv */*/*/*/*/*/*.no 							|| true
rm -fv */*/*/*/*/*/*/*.no 							|| true
