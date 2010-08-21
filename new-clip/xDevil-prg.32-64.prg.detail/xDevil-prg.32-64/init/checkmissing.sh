#!/bin/sh
#

cd temp
export FileToBrowse=
source $xDevil_M_Dir/bin/GetEdit.sh
export Edit=$(cat "$xDevil_S_Dir/Edit.setup.ini" )
export MultiFile=$(cat "$xDevil_S_Dir/MultiFile.setup.ini" )
if [ -f askbin.log ] ; then
	export FileToBrowse="askbin.log "
fi
if [ -f askinclude.log ] ; then
	export FileToBrowse="$FileToBrowse askinclude.log "
fi
if [ -f asklibrary.log ] ; then
	export FileToBrowse="$FileToBrowse asklibrary.log "
fi

if ! [ -z "$FileToBrowse" ] ; then
	if [[ $MultiFile = "Y" ]] ; then
		$Edit $FileToBrowse
	else
		more $FileToBrowse
	fi
	exit 1
fi
