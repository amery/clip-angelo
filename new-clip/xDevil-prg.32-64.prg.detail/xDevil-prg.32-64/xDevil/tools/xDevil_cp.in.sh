#!/bin/bash -u
source $xDevil_I_Dir/xDevilcfg.sh
if [ -f xDevil_cp.ini ] ; then
	printf "configure: creating xDevil_cp$SCRIPTSUFF .."
	exec 															3>xDevil_cp$SCRIPTSUFF
	echo '#!/bin/sh -u' 										>&3
	echo "# Created automatically by 'configure'" 	>&3
	echo 															>&3
	echo "source $xDevil_I_Dir/xDevilcfg.sh" 				>&3
	echo "xDevilROOT=$INSTDIR" 								>&3
	echo "xDevil_ROOT=$INSTDIR" 								>&3
	echo "ADDLIBS='$ADDLIBS'" 								>&3
	echo "ADD_CFLAGS='$ADD_CFLAGS'" 						>&3
	echo "DLLSUFF=$DLLSUFF" 								>&3
	echo "EXESUFF=$EXESUFF" 								>&3
	echo "DLLREALSUFF=$DLLREALSUFF" 						>&3
	echo "STD_LIBDIR=$STD_LIBDIR" 						>&3
	echo "NM_PRG=$NM_PRG" 									>&3
	echo "NM_UNDERSCORE=$NM_UNDERSCORE" 				>&3
	echo "OS_$OSNAME=yes" 									>&3
	echo "OSNAME=$OSNAME" 									>&3
	echo "CC=$CC" 												>&3
	cat xDevil_cp.ini 											>&3
	exec 															3>&-
	chmod +x xDevil_cp$SCRIPTSUFF
	echo ". done."
fi

