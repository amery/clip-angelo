#!/bin/bash -u

export WaitTime=0

rm -fv configure.flags
rm -fv configure.ini
rm -fv license.h
rm -fv Makefile
rm -fv Makefile.ini
rm -fv set.txt
rm -fv tmp.set
rm -fv user home usr.local sys local opt
let tt=0
bb="*"
while [[ $tt -lt 10 ]] ; do
	rm -fv $bb/configur
	rm -fv $bb/configure
	rm -fv $bb/configure.ini
	rm -fv $bb/license.h
	rm -fv $bb/Makefile
	rm -fv $bb/Makefile.ini
	rm -fv $bb/*.no
	rm -fv $bb/*.ok
	rm -fv $bb/set.txt
	rm -fv $bb/tmp.set
	rm -fv $bb/*.o
	rm -fv $bb/*.ex
	rm -fv $bb/*.nm
	rm -fv $bb/*.a
	rm -fv $bb/*.so
	rm -fv $bb/include.clip
	rm -fv $bb/charsets
	rm -fv $bb/*$EXESUFF
	rm -fv $bb/*$EXESUFF.sh
	bb="$bb/*"
	let ++tt
done
