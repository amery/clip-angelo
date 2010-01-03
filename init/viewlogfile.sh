#!/bin/bash 
#
# outputs log files on screen.
#
export LogFileNr=$*
source init/functions.f
[ -f ./configure.ini ] && source ./configure.ini

export MultiFile="n"
export Edit=""
cd $Clip_T_Dir
export FileNameToBrowse=""
FileNameToBrowse="$FileNameToBrowse compile*sh"
if [ -f askbin.log ] ; then
	FileNameToBrowse="$FileNameToBrowse askbin.log"
fi
if [ -f askinclude.log ] ; then
	FileNameToBrowse="$FileNameToBrowse askinclude.log"
fi
if [ -f asklibrary.log ] ; then
	FileNameToBrowse="$FileNameToBrowse asklibrary.log"
fi
source $Clip_M_Dir/init/GetEdit.sh
export Edit=$(cat "$Clip_S_Dir/Edit.setup.ini" )
export MultiFile=$(cat "$Clip_S_Dir/MultiFile.setup.ini" )
export FileToBrowse=""
for nFileName in $FileNameToBrowse ; do
	if [ -f $nFileName ] ; then
		exec                                                3>$Clip_M_Dir/temp/temp$LogFileNr.log
		echo ""                                             >&3
		echo ":::::::::::::::::::::::::::::::::::::::::"    >&3
		banner "TOF"                                        >&3 2>&3 1>&2 0>&1
		echo "  file : $nFileName"                          >&3
		echo ":::::::::::::::::::::::::::::::::::::::::"    >&3
		echo ""                                             >&3
		cat  $nFileName                                     >&3
		echo ""                                             >&3
		echo ":::::::::::::::::::::::::::::::::::::::::"    >&3
		banner "EOF :"                                      >&3 2>&3 1>&2 0>&1
		echo ":::::::::::::::::::::::::::::::::::::::::"    >&3
		echo "                                         "    >&3
		exec                                                3>&-
		sed -e "s/\`/\'/g" <$Clip_M_Dir/temp/temp$LogFileNr.log >$nFileName
		FileToBrowse="$FileToBrowse $nFileName"
	fi
done
if [ "$FileToBrowse"1 != ""1 ] ; then
	export MultiFile="$(cat $Clip_S_Dir/MultiFile.setup.ini )"
	if [ -x $Edit ] ; then
		if [ $MultiFile = "Y" ] ; then
			$Edit $FileToBrowse
		else
			for nFileName in $FileToBrowse ; do
				$Edit $nFileName
			done
		fi
	else
		more *.log
	fi
else
	read -p "No log file. Press enter to continue." >&0
fi
