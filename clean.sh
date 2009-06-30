#!/bin/sh 
if [ -z "$CLIPROOT" ] ; then
	CLIPROOT=`cd ..;pwd`/cliproot
	echo "force CLIPROOT=$CLIPROOT"
else
	echo "use CLIPROOT=$CLIPROOT"
fi
export CLIPROOT
export C64=$(uname -m)
export ARCH=$(uname -m)
if [ -n "$ARCH" ] ; then
	if [[ $ARCH = "x86_64" ]] && [[ $C64 = "x86_64" ]] ; then
		ARCH=x86_64
		C64=64
	else
		ARCH=i586
		C64=""
	fi
fi
export C64
export ARCH

if [ -n "$MAKE" ] ; then
	MAKE="$MAKE"
elif [ -x /usr/local/bin/gmake ] ; then
	MAKE=/usr/local/bin/gmake
elif [ -x /usr/bin/gmake ] ; then
	MAKE=/usr/bin/gmake
else
	MAKE=make
fi
export MAKE
#unset CLIPROOT
pwd=`pwd`

rm -rf debian/tmp debian/clip-lib clip-dev.spec
rm -rf *.bak

cd clip
	if [ -f Makefile ] ; then
		$MAKE clean || true
	else
		$MAKE -f Makefile.in clean
	fi
cd $pwd
cd prg
	./Clean.sh || true
cd $pwd
cd cliplibs
	if [ -f Makefile ] ; then
		$MAKE clean || true
	else
		$MAKE -f Makefile.in clean
	fi
rm -f clip-dev.spec _lst _nlst
cd $pwd
#find . -name '*.ex' -or -name '*.nm' |  xargs rm -f

