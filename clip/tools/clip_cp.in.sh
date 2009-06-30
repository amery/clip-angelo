if [ -f clip_cp.in ]
then

printf "configure: creating clip_cp$SCRIPTSUFF .."
exec 3>clip_cp$SCRIPTSUFF

echo '#!/bin/sh' >&3
echo "# Created automatically by 'configure'" >&3
echo >&3
echo '[ -z "$CLIPROOT" ] && '"CLIPROOT=$CLIPROOT" >&3
echo "CLIP_ROOT=$CLIPROOT" >&3
echo "ADDLIBS='$ADDLIBS'" >&3
echo "ADD_CFLAGS='$ADD_CFLAGS'" >&3
echo "DLLSUFF=$DLLSUFF" >&3
echo "EXESUFF=$EXESUFF" >&3
echo "DLLREALSUFF=$DLLREALSUFF" >&3
echo "STD_LIBDIR=$STD_LIBDIR" >&3
echo "NM_PRG=$NM_PRG" >&3
echo "NM_UNDERSCORE=$NM_UNDERSCORE" >&3
echo "OS_$osname=yes" >&3
echo "OSNAME=$osname" >&3
echo "CC=$CC" >&3

cat clip_cp.in >&3

exec 3>&-
echo ". done."

chmod +x clip_cp$SCRIPTSUFF

fi

