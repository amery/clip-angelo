#!/bin/bash -u

source $Clip_M_Dir/init/functions.f.sh

OnAllScreen $PWD/$0

cd easysoft
./Link.Easysoft.sh
if [[ $? != 0 ]] ; then
	error $0 $LINENO
	exit 1
fi

cd ../libiconv
./Link.libiconv.sh
if [[ $? != 0 ]] ; then
	error $0 $LINENO
	exit 1
fi

cd ../oracle32
./Link.Oracle.sh
if [[ $? != 0 ]] ; then
	error $0 $LINENO
	exit 1
fi

cd ../xclip
./Link.Xclip.sh
if [[ $? != 0 ]] ; then
	error $0 $LINENO
	exit 1
fi

cd ../dbtcp
./Link.dbtcp.sh
if [[ $? != 0 ]] ; then
	error $0 $LINENO
	exit 1
fi

cd ../interbase
./Link.interbase.sh
if [[ $? != 0 ]] ; then
	error $0 $LINENO
	exit 1
fi
