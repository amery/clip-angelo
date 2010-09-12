#!/bin/bash -ux

source $Clip_M_Dir/init/functions.f.sh

OnAllScreen $PWD/$0

mkdir -p$V $PWD/usr/bin
mkdir -p$V $PWD/usr/include
mkdir -p$V $PWD/usr/lib$EXE

if [ -f /opt/easysoft/oob/client/include/sqlucode.h ] ; then
	SourceDir="/opt"
elif [ -f /usr/local/easysoft/oob/client/include/sqlucode.h ] ; then
	SourceDir="/usr/local"
else
	SourceDir=""
fi

if [ -n $SourceDir ] ; then
	if [ -d $SourceDir/easysoft/bin ] ; then
		cp -sf$V $SourceDir/easysoft/bin/* 								$PWD/usr/bin/
	fi

	if [ -d $SourceDir/easysoft/unixODBC/bin ] ; then
		cp -sf$V $SourceDir/easysoft/unixODBC/bin/* 					$PWD/usr/bin/
	fi
	if [ -d $SourceDir/easysoft/unixODBC/include ] ; then
		cp -sf$V $SourceDir/easysoft/unixODBC/include/* 			$PWD/usr/include/
	fi
	if [ -d $SourceDir/easysoft/unixODBC/lib ] ; then
		cp -sf$V $SourceDir/easysoft/unixODBC/lib/* 					$PWD/usr/lib$EXE/
	fi

	if [ -d $SourceDir/easysoft/oob/client ] ; then
		cp -sf$V $SourceDir/easysoft/oob/client/* 					$PWD/usr/lib$EXE/
	fi
	if [ -d $SourceDir/easysoft/oob/client/include ] ; then
		cp -sf$V $SourceDir/easysoft/oob/client/include/* 			$PWD/usr/include/
	fi
	if [ -d $SourceDir/easysoft/oob/client/lib ] ; then
		cp -sf$V $SourceDir/easysoft/oob/client/lib/* 				$PWD/usr/lib$EXE/
	fi

	if [ -d $SourceDir/easysoft/oob/unixODBC/bin ] ; then
		cp -sf$V $SourceDir/easysoft/oob/unixODBC/bin/* 			$PWD/usr/bin/
	fi
	if [ -d $SourceDir/easysoft/oob/unixODBC/include ] ; then
		cp -sf$V $SourceDir/easysoft/oob/unixODBC/include/* 		$PWD/usr/include/
	fi
	if [ -d $SourceDir/easysoft/oob/unixODBC/lib ] ; then
		cp -sf$V $SourceDir/easysoft/oob/unixODBC/lib/* 			$PWD/usr/lib$EXE/
	fi
	#[ -d $HOME/clip$EXE/lib$EXE ] || mkdir -pv $HOME/clip$EXE/lib$EXE
	#[ -d $HOME/clip$EXE/include ] || mkdir -pv $HOME/clip$EXE/include

	#cp -sf$V $PWD/usr/lib$EXE/* 		$HOME/clip$EXE/lib$EXE/
	#cp -sf$V $PWD/usr/include/* 		$HOME/clip$EXE/include/
fi
