#!/bin/bash
xDevil_M_Dir="$PWD"
cd ..
xDevil_H_Dir="$PWD"
cd "$xDevil_M_Dir"
V="v"
MAKE=$(type -p gmake)
xDevil_NAMES=yes
RFLAG=""
dostrip=""
xDevil_CONFIGURE_FLAGS=""
[ -f $xDevil_M_Dir/configure.flags ] && xDevil_CONFIGURE_FLAGS=$(cat $xDevil_M_Dir/configure.flags)
release=""
C64=$(uname -m)
ARCH=$(uname -m)
if [ -n "$ARCH" ] ; then
	if [[ $ARCH = "x86_64" ]] && [[ $C64 = "x86_64" ]] ; then
		ARCH=x86_64
		C64=64
		xDevil_Version=64
	else
		ARCH=i586
		C64=""
		xDevil_Version=32
	fi
fi
option="$*"
xDevilROOT=xDevil$xDevil_Version
if [[ -z "$option" ]] ; then
	option="home"
	DESTDIR="$HOME/"
elif [[ "$option" = "1" ]] || [[ "$option" = "home" ]] || [[ "$option" = "clean" ]] ; then
	option="home"
	DESTDIR="$HOME/"
	INSTDIR=$DESTDIR$xDevilROOT
	export xDevil_I_Dir="$INSTDIR/include/"
	export xDevil_B_Dir="$INSTDIR/bin/"
	export xDevil_D_Dir="$INSTDIR/doc/"
	export xDevil_L_Dir="$INSTDIR/lib$C64/"
	mkdir -p$V $xDevil_B_Dir || true
	touch $xDevil_B_Dir/0 || true
	if ! [[ -f "$xDevil_B_Dir/0" ]] ; then
		echo "Yoy have NO WRITE RIGHTS on $INSTDIR"
	fi
	rm -f $xDevil_B_Dir/0
elif [[ "$option" = "2" ]] || [[ "$option" = "local" ]] ; then
	option="local"
	DESTDIR="$xDevil_H_Dir/"
	INSTDIR=$DESTDIR$xDevilROOT
	export xDevil_I_Dir="$INSTDIR/include/"
	export xDevil_B_Dir="$INSTDIR/bin/"
	export xDevil_D_Dir="$INSTDIR/doc/"
	export xDevil_L_Dir="$INSTDIR/lib$C64/"
	mkdir -p$V $xDevil_B_Dir || true
	touch $xDevil_B_Dir/0 || true
	if ! [[ -f "$xDevil_B_Dir/0" ]] ; then
		echo "Yoy have NO WRITE RIGHTS on $INSTDIR"
	fi
	rm -f $xDevil_B_Dir/0
elif [[ "$option" = "3" ]] || [[ "$option" = "opt" ]] ; then
	option="opt"
	DESTDIR="/opt/"
	INSTDIR=$DESTDIR$xDevilROOT
	export xDevil_I_Dir="$INSTDIR/include/"
	export xDevil_B_Dir="$INSTDIR/bin/"
	export xDevil_D_Dir="$INSTDIR/doc/"
	export xDevil_L_Dir="$INSTDIR/lib$C64/"
	mkdir -p$V $xDevil_B_Dir || true
	touch $xDevil_B_Dir/0 || true
	if ! [[ -f "$xDevil_B_Dir/0" ]] ; then
		echo "Yoy have NO WRITE RIGHTS on $INSTDIR"
	fi
	rm -f $xDevil_B_Dir/0
elif [[ "$option" = "4" ]] || [[ "$option" = "usr.local" ]] ; then
	option="usr.local"
	DESTDIR="/usr/local/"
	INSTDIR=$DESTDIR$xDevilROOT
	export xDevil_I_Dir="$INSTDIR/include/"
	export xDevil_B_Dir="$INSTDIR/bin/"
	export xDevil_D_Dir="$INSTDIR/doc/"
	export xDevil_L_Dir="$INSTDIR/lib$C64/"
	mkdir -p$V $xDevil_B_Dir || true
	touch $xDevil_B_Dir/0 || true
	if ! [[ -f "$xDevil_B_Dir/0" ]] ; then
		echo "Yoy have NO WRITE RIGHTS on $INSTDIR"
	fi
	rm -f $xDevil_B_Dir/0
#elif [[ "$option" = "5" ]] || [[ "$option" = "sys" ]] ; then
#	option="sys"
#	DESTDIR="/usr/bin/"
#	INSTDIR=$DESTDIR$xDevilrOOT
#	export xDevil_I_Dir="usr/include/$xDevilROOT"
#	export xDevil_B_Dir="$DESTDIR/"
#	export xDevil_D_Dir="usr/doc/$xDevilROOT"
#	export xDevil_L_Dir="usr/lib$C64/"
#	mkdir -p$V $xDevil_B_Dir || true
#	touch $xDevil_B_Dir/0 || true
#	if ! [[ -f "$xDevil_B_Dir/0" ]] ; then
#		echo "Yoy have NO WRITE RIGHTS on $INSTDIR"
#		exit 1
#	fi
#	rm -f $xDevil_B_Dir/0
else
#	echo
#	echo "$0 options are : [ '' | home(default) | sys | opt | usr.local ]"
#	exit 1
	option=1
	source $xDevil_M_Dir/init/start.sh $option
fi
#export CONFIGUR_SH=$xDevil_M_Dir/init/config.sh
export option
export ARCH
export C64
export xDevil_CONFIGURE_FLAGS
export xDevil_NAMES
export xDevilROOT
export DESTDIR
export BINDIR=$HOME/bin
export INSTDIR="$DESTDIR$xDevilROOT"
export EXESUFF=$xDevil_Version
export MAKE
export Making="$option"
export SCRIPTSUFF=$xDevil_Version.sh
# xDevil-prg
export xDevil_M_Dir
export xDevil_T_Dir="$xDevil_M_Dir/temp/"
export xDevil_C_Dir="$xDevil_M_Dir/config/"
export xDevil_S_Dir="$xDevil_C_Dir/setup/"
export LibDir="/lib$C64/"
# install directories
mkdir -p$V $xDevil_B_Dir
echo "#!/bin/sh" 								>$xDevil_B_Dir/linkall.sh
echo "" 											>>$xDevil_B_Dir/linkall.sh
echo "mkdir -p$V $HOME/bin" 				>>$xDevil_B_Dir/linkall.sh
echo "cd $HOME/bin" 							>>$xDevil_B_Dir/linkall.sh
echo "ln -sf$V $INSTDIR/bin/* ./" 		>>$xDevil_B_Dir/linkall.sh

chmod +x $xDevil_B_Dir/linkall.sh
#set >start.set.txt