#!/bin/bash -u
source $Clip_I_Dir/clipcfg.sh
if [ -f clip_msgmerge.ini ] ; then
	printf "configure: creating clip_msgmerge$SCRIPTSUFF .."
	exec  															3>clip_msgmerge$SCRIPTSUFF
	echo '#!/bin/sh ' 											>&3
	echo "# Created automatically by 'configure'" 		>&3
	echo 																>&3
	echo "source $Clip_I_Dir/clipcfg.sh" 					>&3
	echo "CLIPROOT=$INSTDIR" 									>&3
	echo "NO_GETTEXT=$NO_GETTEXT" 							>&3
	if [ "$PO_COMPAT" = yes ] ; then
		echo "PO_COMPAT=yes" 									>&3
	fi
	echo "PO_FROM_COMPAT=\"$PO_FROM_COMPAT\"" 			>&3
	echo "PO_TO_COMPAT=\"$PO_TO_COMPAT\"" 					>&3
	cat clip_msgmerge.ini 										>&3
	exec 																3>&-
	echo ". done.>&1"
	chmod +x clip_msgmerge$SCRIPTSUFF
fi
