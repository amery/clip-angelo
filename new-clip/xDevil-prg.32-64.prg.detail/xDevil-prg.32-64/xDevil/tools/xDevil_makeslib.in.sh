#!/bin/bash -u
source $xDevil_I_Dir/xDevilcfg.sh
if [ -f xDevil_makeslib.ini ] ; then
	printf "configure: creating xDevil_makeslib$SCRIPTSUFF .."
	exec 																3>xDevil_makeslib$SCRIPTSUFF
	echo '#!/bin/sh -u' 											>&3
	echo "# Created automatically by 'configure'" 		>&3
	echo 																>&3
	echo "xDevilROOT=$INSTDIR" 									>&3
	echo "xDevil_ROOT=$INSTDIR"									>&3
	echo "ADDLIBS='$ADDLIBS'" 									>&3
	echo "ADD_CFLAGS='$ADD_CFLAGS'" 							>&3
	echo "DLLSUFF=$DLLSUFF" 									>&3
	echo "EXESUFF=$EXESUFF" 									>&3
	echo "DLLREALSUFF=$DLLREALSUFF" 							>&3
	echo "STD_LIBDIR=$STD_LIBDIR" 							>&3
	echo "NM_PRG=$NM_PRG" 										>&3
	echo "NM_UNDERSCORE=$NM_UNDERSCORE" 					>&3
	echo "OS_$OSNAME=yes" 										>&3
	echo "OSNAME=$OSNAME"										>&3
	echo "CC=$CC"													>&3
	if [ "$USE_TASKS" = yes ] ; then
		echo 'USE_TASKS=yes'										>&3
	fi
	cat xDevil_makeslib.ini										>&3
	exec 																3>&-
	chmod +x xDevil_makeslib$SCRIPTSUFF
	echo ". done."
fi

