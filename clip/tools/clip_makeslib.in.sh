#!/bin/bash -u
source $Clip_I_Dir/clipcfg.sh
if [ -f clip_makeslib.ini ] ; then
	printf "configure: creating clip_makeslib$SCRIPTSUFF .."
	exec 																3>clip_makeslib$SCRIPTSUFF
	echo '#!/bin/bash -u ' 											>&3
	echo "# Created automatically by 'configure'" 		>&3
	echo 																>&3
	echo "CLIPROOT=$INSTDIR" 									>&3
	echo "CLIP_ROOT=$INSTDIR"									>&3
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
	cat clip_makeslib.ini										>&3
	exec 																3>&-
	chmod +x clip_makeslib$SCRIPTSUFF
	echo ". done."
fi

