#!/bin/bash -u
#
# outputs log files on screen.
#
LogFileNr=$*
[ -f ./configure.ini ] && source ./configure.ini
source $Clip_F_F
source $Clip_M_Dir/bin/GetEdit.sh
export Edit=$(cat "$Clip_S_Dir/Edit.setup.ini" )
export MultiFile=$(cat "$Clip_S_Dir/MultiFile.setup.ini" )
export LogFileErr="$Clip_M_Dir/temp/compile.err.$LogFileNr.sh"
export LogFileLog="$Clip_M_Dir/temp/compile.log.$LogFileNr.sh"
#OnScreen 0 "Nr : "$LogFileNr
[ -f $LogFileLog ] && OnScreen 0 "LogFileLog : "$LogFileLog
[ -f $LogFileErr ] && OnScreen 0 "LogFileErr : "$LogFileErr
export FileToBrowse=
if [ -f $LogFileLog ] ; then
	export FileToBrowse="$FileToBrowse $LogFileLog"
	exec                                                3>$Clip_M_Dir/temp/temp$LogFileNr.log
	echo ""                                             >&3
	echo ":::::::::::::::::::::::::::::::::::::::::"    >&3
	banner "TOF"                                        >&3 2>&3 1>&2 0>&1
	echo "  file : $LogFileLog"                         >&3
	echo ":::::::::::::::::::::::::::::::::::::::::"    >&3
	echo ""                                             >&3
	cat  $LogFileLog                                    >&3
	echo ""                                             >&3
	echo ":::::::::::::::::::::::::::::::::::::::::"    >&3
	banner "EOF :"                                      >&3 2>&3 1>&2 0>&1
	echo ":::::::::::::::::::::::::::::::::::::::::"    >&3
	echo "                                         "    >&3
	exec                                                3>&-
	sed -e "s/\`/\'/g" <$Clip_M_Dir/temp/temp$LogFileNr.log >$LogFileLog
	let ++LogFileNr
fi

if [ -f $LogFileErr ] ; then
	export FileToBrowse="$FileToBrowse $LogFileErr"
	exec                                                 3>$Clip_M_Dir/temp/temp$LogFileNr.log
	echo "                                         "     >&3
	echo ":::::::::::::::::::::::::::::::::::::::::"     >&3
	banner "TOF"                                         >&3 2>&3 1>&2 0>&1
	echo "  file : $LogFileErr "                         >&3
	echo ":::::::::::::::::::::::::::::::::::::::::"     >&3
	echo ""                                              >&3
	cat $LogFileErr                                      >&3
	echo ""                                              >&3
	echo ":::::::::::::::::::::::::::::::::::::::::"     >&3
	banner "EOF :"                                       >&3 2>&3 1>&2 0>&1
	echo ":::::::::::::::::::::::::::::::::::::::::"     >&3
	echo ""                                              >&3
	exec                                                 3>&-

	sed -e "s/\`/\'/g" <$Clip_M_Dir/temp/temp$LogFileNr.log >$LogFileErr
fi

if [ -f $Clip_M_Dir/temp/com.sh ] ; then
	export FileToBrowse="$FileToBrowse $Clip_M_Dir/temp/com.sh"
	exec                                                 3>$Clip_M_Dir/temp/temp$LogFileNr.log
	echo ""                                              >&3
	echo ":::::::::::::::::::::::::::::::::::::::::"     >&3
	banner "TOF"                                         >&3 2>3 1>2 0>1
	echo "  file : $com:sh"                              >&3
	echo ":::::::::::::::::::::::::::::::::::::::::"     >&3
	echo ""                                              >&3
	cat $Clip_M_Dir/temp/com.sh                          >&3
	echo "                                         "     >&3
	echo ":::::::::::::::::::::::::::::::::::::::::"     >&3
	banner "EOF :"                                       >&3 2>3 1>2 0>1
	echo ":::::::::::::::::::::::::::::::::::::::::"     >&3
	echo ""                                              >&3
	exec                                                 3>&-

	sed -e "s/\`/\'/g" <$Clip_M_Dir/temp/temp$LogFileNr.log >$Clip_M_Dir/temp/com.sh
fi
rm -f$V $Clip_M_Dir/temp/temp$LogFileNr.log
if ! [ -z "$FileToBrowse" ] ; then
	if [[ $MultiFile = "Y" ]] ; then
		$Edit $FileToBrowse
	else
		more $FileToBrowse
	fi
	exit 1
fi
