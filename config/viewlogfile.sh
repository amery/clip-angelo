#!/bin/bash -ue
#
# outputs log files on screen.
#
export LogFileNr=$*
source $Clip_F_F
[ -f ./configure.ini ] && source ./configure.ini

export MultiFile="n"
export Edit=""
cd $Clip_T_Dir
export FileNameToBrowse=""
if [ -f compile.err.0.sh ] ; then
	export FileNameToBrowse="$FileNameToBrowse compile.err.0.sh"
fi
if [ -f compile.log.0.sh ] ; then
	export FileNameToBrowse="$FileNameToBrowse compile.log.0.sh"
fi
if [ -f askbin.log ] ; then
	export FileNameToBrowse="$FileNameToBrowse askbin.log"
fi
if [ -f askinclude.log ] ; then
	export FileNameToBrowse="$FileNameToBrowse askinclude.log"
fi
if [ -f asklibrary.log ] ; then
	export FileNameToBrowse="$FileNameToBrowse asklibrary.log"
fi
source $Clip_M_Dir/bin/GetEdit.sh
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
		export FileToBrowse="$FileToBrowse $nFileName"
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
