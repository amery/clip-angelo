#!/bin/bash -u

source $Clip_M_Dir/init/functions.f.sh

OnAllScreen $PWD/$0

mkdir -p$V $PWD/usr/bin
mkdir -p$V $PWD/usr/include
mkdir -p$V $PWD/usr/lib$EXE

if [ -f /opt/iconv/bin/iconv ] ; then
	SourceDir="/opt/iconv"
elif [ -f /usr/local/bin/iconv ] ; then
	SourceDir="/usr/local"
else
	SourceDir=""
fi

if [ -n $SourceDir ] ; then
	cp -sf$V $SourceDir/bin/iconv 						$PWD/usr/bin/

	cp -sf$V $SourceDir/include/iconv.h 				$PWD/usr/include/
	cp -sf$V $SourceDir/include/libcharset.h 			$PWD/usr/include/
	cp -sf$V $SourceDir/include/localcharset.h 		$PWD/usr/include/

	cp -sf$V $SourceDir/lib/charset.alias 					$PWD/usr/lib$EXE/
	cp -sf$V $SourceDir/lib/libcharset.a 					$PWD/usr/lib$EXE/
	cp -sf$V $SourceDir/lib/libcharset.la 					$PWD/usr/lib$EXE/
	cp -sf$V $SourceDir/lib/libcharset.so 					$PWD/usr/lib$EXE/
	cp -sf$V $SourceDir/lib/libcharset.so.1 				$PWD/usr/lib$EXE/
	cp -sf$V $SourceDir/lib/libcharset.so.1.0.0 			$PWD/usr/lib$EXE/
	cp -sf$V $SourceDir/lib/libiconv.la 					$PWD/usr/lib$EXE/
	cp -sf$V $SourceDir/lib/libiconv.so 					$PWD/usr/lib$EXE/
	cp -sf$V $SourceDir/lib/libiconv.so.2 					$PWD/usr/lib$EXE/
	cp -sf$V $SourceDir/lib/libiconv.so.2.5.0 			$PWD/usr/lib$EXE/
	cp -sf$V $SourceDir/lib/preloadable_libiconv.so 	$PWD/usr/lib$EXE/

#	[ -d $HOME/clip$EXE/lib$EXE ] || mkdir -pv $HOME/clip$EXE/lib$EXE
#	[ -d $HOME/clip$EXE/include ] || mkdir -pv $HOME/clip$EXE/include

#	cp -sf$V $PWD/usr/lib$EXE/* 		$HOME/clip$EXE/lib$EXE/	|| true
#	cp -sf$V $PWD/usr/include/* 		$HOME/clip$EXE/include/	|| true
fi
