#!/bin/sh
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

if [ -n "$MAKE" ]
then
	MAKE="$MAKE"
elif [ -x /usr/local/bin/gmake  ]
then
	MAKE=/usr/local/bin/gmake
elif [ -x /usr/bin/gmake ]
then
	MAKE=/usr/bin/gmake
else
	MAKE=/usr/bin/make
fi

export MAKE
#unset CLIPROOT

#source config/gen.in
$MAKE $*
