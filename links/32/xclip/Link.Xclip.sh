#!/bin/bash -u

source $Clip_M_Dir/init/functions.f.sh

OnAllScreen $PWD/$0

mkdir -p$V $PWD/usr/bin
mkdir -p$V $PWD/usr/include
mkdir -p$V $PWD/usr/lib$EXE

if [ -f /opt/xclip/bin/xclip ] ; then
	SourceDir="/opt/xclip"
elif [ -f /usr/local/bin/xclip ] ; then
	SourceDir="/usr/local"
else
	SourceDir=""
fi

if [ -n $SourceDir ] ; then
	cp -sf$V $SourceDir/bin/xclip* 					$PWD/usr/bin/

#	[ -d $HOME/clip$EXE/bin ] 		|| mkdir -pv $HOME/clip$EXE/bin

#	cp -sf$V $PWD/usr/bin/* 		$HOME/clip$EXE/bin/
fi
