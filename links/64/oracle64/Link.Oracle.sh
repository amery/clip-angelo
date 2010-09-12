#!/bin/bash -u

source $Clip_M_Dir/init/functions.f.sh

OnAllScreen $PWD/$0

mkdir -p$V $PWD/usr/bin
mkdir -p$V $PWD/usr/include
mkdir -p$V $PWD/usr/lib$EXE

cp -sf$V /usr/include/oracle/*/client64/* $PWD/usr/include/

cp -sf$V /usr/lib/oracle/*/client64/lib/* $PWD/usr/lib64

#[ -d $HOME/clip$EXE/lib$EXE ] || mkdir -pv $HOME/clip$EXE/lib$EXE
#[ -d $HOME/clip$EXE/include ] || mkdir -pv $HOME/clip$EXE/include

#cp -sf$V $PWD/usr/lib$EXE/* 	$HOME/clip$EXE/lib$EXE/	|| true
#cp -sf$V $PWD/usr/include/* 	$HOME/clip$EXE/include/	|| true
