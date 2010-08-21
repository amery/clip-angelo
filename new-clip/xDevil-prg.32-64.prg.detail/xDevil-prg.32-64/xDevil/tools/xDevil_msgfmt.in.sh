#!/bin/bash -u
source $xDevil_I_Dir/xDevilcfg.sh
if [ -f xDevil_msgfmt.ini ] ; then
	printf "configure: creating xDevil_msgfmt$SCRIPTSUFF .."
	exec 																3>xDevil_msgfmt$SCRIPTSUFF
	echo '#!/bin/sh  ' 											>&3
	echo "# Created automatically by 'configure'" 		>&3
	echo 																>&3
	echo "source $xDevil_I_Dir/xDevilcfg.sh" 					>&3
	echo "xDevilROOT=$INSTDIR" 									>&3
	echo "NO_GETTEXT=$NO_GETTEXT" 							>&3
	if [ "$PO_COMPAT" = yes ] ; then
		echo "PO_COMPAT=yes" 									>&3
	fi
	echo "PO_FROM_COMPAT=\"$PO_FROM_COMPAT\"" 			>&3
	echo "PO_TO_COMPAT=\"$PO_TO_COMPAT\"" 					>&3
	cat xDevil_msgfmt.ini 											>&3
	exec 																3>&-
	chmod +x xDevil_msgfmt$SCRIPTSUFF
	echo ". done."
fi

