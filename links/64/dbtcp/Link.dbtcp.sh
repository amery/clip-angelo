#!/bin/bash -u

source $Clip_M_Dir/init/functions.f.sh

OnAllScreen $PWD/$0

mkdir -p$V $PWD/usr/bin
mkdir -p$V $PWD/usr/include
mkdir -p$V $PWD/usr/lib$EXE

if [ -f /opt/dbtcp/include/protocol.h ] ; then
	SourceDir="/opt"
elif [ -f /usr/local/dbtcp/include/protocol.h ] ; then
	SourceDir="/usr/local"
else
	SourceDir=""
fi

if [ -n $SourceDir ] ; then
	cp -sf$V $SourceDir/dbtcp/include/* 		$PWD/usr/include/

	cp -sf$V $SourceDir/dbtcp/libs/* 			$PWD/usr/lib$EXE/

	#[ -d $HOME/clip$EXE/lib$EXE ] || mkdir -pv $HOME/clip$EXE/lib$EXE
	#[ -d $HOME/clip$EXE/include ] || mkdir -pv $HOME/clip$EXE/include

	#cp -sf$V $PWD/usr/lib$EXE/* 		$HOME/clip$EXE/lib$EXE/
	#cp -sf$V $PWD/usr/include/* 		$HOME/clip$EXE/include/
fi
