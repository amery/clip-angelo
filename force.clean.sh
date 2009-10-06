#!/bin/sh

rm -fv Makefile 							Makefile.ini  														configure.ini
rm -fv */Makefile 						*/Makefile.ini 				*/configure 					*/configure.ini
rm -fv */*/Makefile 						*/*/Makefile.ini 				*/*/configure 					*/*/*/configure.ini
rm -fv */*/*/Makefile 					*/*/*/Makefile.ini 			*/*/*/configure 				*/*/*/configure.ini
rm -fv */*/*/*/Makefile 				*/*/*/*/Makefile.ini 		*/*/*/*/configure 			*/*/*/*/configure.ini
rm -fv */*/*/*/*/Makefile 				*/*/*/*/Makefile.ini 		*/*/*/*/*/configure 			*/*/*/*/*/configure.ini
rm -fv */*/*/*/*/*/Makefile 			*/*/*/*/*/*/Makefile.ini 	*/*/*/*/*/*/configure		*/*/*/*/*/*/configure.ini
rm -fv */*/*/*/*/*/*/Makefile 		*/*/*/*/*/*/*/Makefile.ini */*/*/*/*/*/*/configure 	*/*/*/*/*/*/*/configure.ini
rm -fv *.ok
rm -fv */*.ok
rm -fv */*/*.ok
rm -fv */*/*/*.ok
rm -fv */*/*/*/*.ok
rm -fv */*/*/*/*/*.ok
rm -fv */*/*/*/*/*/*.ok
rm -fv */*/*/*/*/*/*/*.ok
./clean.sh
