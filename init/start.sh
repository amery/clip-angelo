#!/bin/bash -ux
Clip_M_Dir="$PWD"
cd ..
Clip_H_Dir="$PWD"
cd "$Clip_M_Dir"
V="v"
MAKE=$(type -p gmake)
CLIP_NAMES=yes
RFLAG=""
dostrip=""
CLIP_CONFIGURE_FLAGS=""
[ -f $Clip_M_Dir/configure.flags ] && CLIP_CONFIGURE_FLAGS=$(cat $Clip_M_Dir/configure.flags)
release=""
C64=$(uname -m)
ARCH=$(uname -m)
if [ -n "$ARCH" ] ; then
	if [[ $ARCH = "x86_64" ]] && [[ $C64 = "x86_64" ]] ; then
		ARCH=x86_64
		C64=64
		Clip_Version=64
	else
		ARCH=i586
		C64=""
		Clip_Version=32
	fi
fi
option="$*"
CLIPROOT=/clip$Clip_Version/
if [[ -z "$option" ]] ; then
	option="home"
	DESTDIR="$HOME/"
elif [[ "$option" = "1" ]] || [[ "$option" = "home" ]] || [[ "$option" = "clean" ]] ; then
	option="home"
	DESTDIR="$HOME/"
	INSTDIR=$DESTDIR$CLIPROOT
	export Clip_I_Dir="$INSTDIR/include/"
	export Clip_B_Dir="$INSTDIR/bin/"
	export Clip_D_Dir="$INSTDIR/doc/"
	export Clip_L_Dir="$INSTDIR/lib$C64/"
	mkdir -p$V $Clip_B_Dir || true
	touch $Clip_B_Dir/0 || true
	if ! [[ -f "$Clip_B_Dir/0" ]] ; then
		echo "Yoy have NO WRITE RIGHTS on $INSTDIR"
		exit 1
	fi
	rm -f $Clip_B_Dir/0
elif [[ "$option" = "2" ]] || [[ "$option" = "local" ]] ; then
	option="local"
	DESTDIR="$Clip_H_Dir/"
	INSTDIR=$DESTDIR$CLIPROOT
	export Clip_I_Dir="$INSTDIR/include/"
	export Clip_B_Dir="$INSTDIR/bin/"
	export Clip_D_Dir="$INSTDIR/doc/"
	export Clip_L_Dir="$INSTDIR/lib$C64/"
	mkdir -p$V $Clip_B_Dir || true
	touch $Clip_B_Dir/0 || true
	if ! [[ -f "$Clip_B_Dir/0" ]] ; then
		echo "Yoy have NO WRITE RIGHTS on $INSTDIR"
		exit 1
	fi
	rm -f $Clip_B_Dir/0
elif [[ "$option" = "3" ]] || [[ "$option" = "opt" ]] ; then
	option="opt"
	DESTDIR="/opt/"
	INSTDIR=$DESTDIR$CLIPROOT
	export Clip_I_Dir="$INSTDIR/include/"
	export Clip_B_Dir="$INSTDIR/bin/"
	export Clip_D_Dir="$INSTDIR/doc/"
	export Clip_L_Dir="$INSTDIR/lib$C64/"
	mkdir -p$V $Clip_B_Dir || true
	touch $Clip_B_Dir/0 || true
	if ! [[ -f "$Clip_B_Dir/0" ]] ; then
		echo "Yoy have NO WRITE RIGHTS on $INSTDIR"
		exit 1
	fi
	rm -f $Clip_B_Dir/0
elif [[ "$option" = "4" ]] || [[ "$option" = "usr.local" ]] ; then
	option="usr.local"
	DESTDIR="/usr/local/"
	INSTDIR=$DESTDIR$CLIPROOT
	export Clip_I_Dir="$INSTDIR/include/"
	export Clip_B_Dir="$INSTDIR/bin/"
	export Clip_D_Dir="$INSTDIR/doc/"
	export Clip_L_Dir="$INSTDIR/lib$C64/"
	mkdir -p$V $Clip_B_Dir || true
	touch $Clip_B_Dir/0 || true
	if ! [[ -f "$Clip_B_Dir/0" ]] ; then
		echo "Yoy have NO WRITE RIGHTS on $INSTDIR"
		exit 1
	fi
	rm -f $Clip_B_Dir/0
#elif [[ "$option" = "5" ]] || [[ "$option" = "sys" ]] ; then
#	option="sys"
#	DESTDIR="/usr/bin/"
#	INSTDIR=$DESTDIR$CLIPROOT
#	export Clip_I_Dir="usr/include/$CLIPROOT"
#	export Clip_B_Dir="$DESTDIR/"
#	export Clip_D_Dir="usr/doc/$CLIPROOT"
#	export Clip_L_Dir="usr/lib$C64/"
#	mkdir -p$V $Clip_B_Dir || true
#	touch $Clip_B_Dir/0 || true
#	if ! [[ -f "$Clip_B_Dir/0" ]] ; then
#		echo "Yoy have NO WRITE RIGHTS on $INSTDIR"
#		exit 1
#	fi
#	rm -f $Clip_B_Dir/0
else
	echo
	echo "$0 options are : [ '' | home(default) | sys | opt | usr.local ]"
	exit 1
fi
#export CONFIGUR_SH=$Clip_M_Dir/init/config.sh
export ARCH
export C64
export CLIP_CONFIGURE_FLAGS
export CLIP_NAMES
export CLIPROOT
export DESTDIR
export BINDIR=$HOME/bin
export INSTDIR="$DESTDIR$CLIPROOT"
export EXESUFF=$Clip_Version
export MAKE
export Making="$option"
export SCRIPTSUFF=$Clip_Version.sh
# clip-prg
export Clip_M_Dir
export Clip_T_Dir="$Clip_M_Dir/temp/"
export Clip_C_Dir="$Clip_M_Dir/config/"
export Clip_S_Dir="$Clip_C_Dir/setup/"
export LibDir="/lib$C64/"
# install directories
mkdir -p$V $Clip_B_Dir
echo "#!/bin/sh" 								>$Clip_B_Dir/linkall.sh
echo "" 											>>$Clip_B_Dir/linkall.sh
echo "mkdir -p$V $HOME/bin" 				>>$Clip_B_Dir/linkall.sh
echo "cd $HOME/bin" 							>>$Clip_B_Dir/linkall.sh
echo "ln -sf$V $INSTDIR/bin/* ./" 		>>$Clip_B_Dir/linkall.sh

chmod +x $Clip_B_Dir/linkall.sh
set >start.set.txt