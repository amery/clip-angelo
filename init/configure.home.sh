#!/bin/sh -u
#
#	Configure script for clip
#	Paul Lasarev - 2000.
#

#
# added by Angelo for 64 bits compilation
#
#
# added
#
#source $Clip_M_Dir/init/start.sh "$HOME"
#source $Clip_M_Dir/init/functions.f
#export ARCH="$(GetArch)"
cd clip
FORCEALIGN=""
MLIB=""
MDBG=""
osname="LINUX"
OPTFLAGS=""
STATICLINK=""
NO_GETTEXT=""
CC=""
AS_PRG=""
NM_PRG=""
CHECK_STACK=""
PO_COMPAT=""
SCRIPTSUFF=$C64.sh
DEBUGFLAGS=""
STD_LIBDIR=""
STD_LIB_DIR=/usr/lib$C64
DLLSUFF=".so"
DLLREALSUFF=".so"
CLIP_DLLIMPORT=""
CLIP_DLLEXPORT=""
NM_UNDERSCORE=""
EXESUFF=$C64
USE_WCHARS=yes
USE_TASKS=yes
#if [ "$CLIP_TASKS" ]
#then
#	USE_TASKS=$CLIP_TASKS
#fi
USE_LD=no
USE_AS=yes
XCLIP=""
pwd=`pwd`

if [ X"$1" = X"-f" -a -f "configure.flags" ] ; then
	opts=`cat configure.flags`
else
	opts="$*"
fi

#read -p "Parameters : $opts"
echo $opts > configure.flags

rm -f$V Makefile Makefile.inc clipcfg.sh

##############################"""""
###########oneofdir()
###########{
###########	for i in $* ; 	do
###########		if [ -d $i ] ; 		then
###########			echo $i
###########			return 0
###########		fi
###########	done
###########	return 0
###########}

###########xoneof()
###########{
###########	for i in $* ; 	do
###########		if [ -x $i ] ; then
###########			echo $i
###########			return 0
###########		fi
######################	done
###########	return 0
###########}

USE_MEMDBG=no
MEMDEBUG_LEVEL=0

#for opt in $opts ; do
#	case "$opt" in 
#	-f)
#		;;
#	-m=*)
#		MEMDEBUG_LEVEL=`echo "$opt" | sed -e "s/^-m=//"`
#		USE_MEMDBG=yes
#		#rm -f Makefile.inc clipcfg.sh
#		;;
#	-m)
#		MEMDEBUG_LEVEL=3
#		USE_MEMDBG=yes
#		#rm -f Makefile.inc clipcfg.sh
#		;;
#	-[oO]*)
#		rm -f$V Makefile.inc
#		OPTFLAGS=-O2
#		;;
#	-[cC]*)
#		rm -f$V Makefile.inc clipcfg.sh
#		;;
#	-[rR]*)
#		#rm -f$V Makefile.inc
#		[ -z "$CLIPROOT" ] && CLIPROOT=/usr/local/clip$C4
#		[ -z "$BINDIR" ] && BINDIR=/usr/local/bin
#		OPTFLAGS=-O2
#		DEBUGFLAGS=""
#		STD_LIBDIR="yes"
#		USE_WCHARS=""
#		;;
#	-[sS])
#		STD_LIBDIR="yes"
#		;;
#	-l)
#		STD_LIB_DIR=/usr/local/lib
#		STD_LIBDIR="yes"
#		;;
#	-a)
#		FORCEALIGN=4
#		;;
#	-[tT]*)
#		USE_TASKS="no"
#		;;
#	-stack)
#		CHECK_STACK=yes
#		;;
#	-mingw-cross)
#		CC=`xoneof /usr/bin/i586-mingw32msvc-gcc /opt/cross-tools/bin/i386-mingw32msvc-gcc`
#		CROSSROOT=`oneofdir /usr/i586-mingw32msvc /opt/cross-tools/i386-mingw32msvc`
#
#		test -n "$CROSSROOT" || CROSSROOT=/usr/bin/i586-mingw32msvc-gcc
#
#		CC="$CC -mms-bitfields -march=pentium"
#		AS_PRG="$CROSSROOT/bin/as"
#
#		USE_AS=yes
#		LD_PRG="$CROSSROOT/bin/ld"
#		USE_LD=yes
#		NM_PRG="$CROSSROOT/bin/nm"
#		CLIPROOT=/clip
#		BINDIR=/windows/command
#		STD_LIBDIR="yes"
#		OSNAME=MINGW
#		;;
#	0)
#		;;
#	*)
#		echo "usage: ./configure [-o] [-m] [-c] [-r] [-s] [-t]"
#		echo "	-o  for optimisation"
#		echo "	-m  for memdebug"
#		echo "	-m=<LEVEL> for memdebug with level <LEVEL>"
#		echo "	-c  to clean cache"
#		echo "	-r  release options"
#		echo "	-s  use standard libdir (/usr/lib)"
#		echo "	-l  set standard libdir to /usr/local/lib"
#		echo "	-a  force align code"
#		echo "	-t  do NOT use tasks code"
#		echo "	-stack  enable runtime stack check"
#		echo "	-mingw-cross  configure for cross-compile on mingw32"
#		exit 0
##		;;
#	esac
#done


if [ "$USE_MEMDBG" = yes ] ; then
	rm -f Makefile.inc
	MEMDEBUG="$MEMDEBUG_LEVEL"
	#${opt#*=}
	DBGLIB=memdebug/memdebug.a
	MEMDBGFLAGS="-DMEMDBG -Imemdebug"
	(cd memdebug; ./configure.sh; make)
	MDBG="-DMEMDEBUG=$(eval $(memdebug/malloc_dbg -f memdebug/mallocrc -b ${MEMDEBUG} 2>&-); echo MALLOC_DEBUG ) -DMALLOC_FUNC_CHECK"
	MLIB=memdebug/memdebug.a
fi

#arch="$HOSTTYPE"
#[ -z "$arch" ] &&
arch=`uname -m 2>/dev/null`
[ -z "$arch" ] && arch=`uname -p 2>/dev/null`
[ -z "$arch" ] && arch=unknown

case "$arch" in
i?86)
	arch="i386"
	;;
*)
	;;
esac



[ -z "$CLIPROOT" ] && CLIPROOT=`cd ../..;pwd`/cliproot
#[ -z "$BINDIR" ] &&
BINDIR=$HOME/bin
rm -Rf$V $DESTDIR$CLIPROOT
#	initial CFLAGS
#
#C_FLAGS="-Wall -I. $DEBUGFLAGS $OPTFLAGS $MDBG"
C_FLAGS="-Wall -fPIC -I$Clip_I_Dir $DEBUGFLAGS $OPTFLAGS"
C_LIBS="-L$Clip_L_Dir -lclip"
ADD_CFLAGS="-fPIC"
CLIPFLAGS="-w -l -O -I$Clip_I_Dir"

uname=`uname -s`
uver=`uname -r`

####remark from Uri: "hostname -f" on Solaris changed hostname !
####                 and "hname" not used in this script.
#hname=`hostname -f 2>/dev/null`
#if [ $? != 0 ] ; then
#	hname=`hostname 2>/dev/null`
#	if [ $? != 0 ] ; then
#		hname=`uname -n 2>/dev/null`
#		if [ $? != 0 ] ; then
#			hname=localhost
#		fi
#	fi
#fi

if [ -z "$osname" ] ; then
osname=UNIX
case "$uname" in
	BSD/OS*|bsdi*)
		osname=BSDI
		;;
	[lL]inux*)
		osname=LINUX
		;;
	FreeBSD*)
		osname=FREEBSD
		STATICLINK=-static
		;;
	NetBSD*)
		osname=NETBSD
		;;
	OpenBSD*)
		osname=OPENBSD
		;;
	SunOS*)
		osname=SUNOS
		;;
	CYGWIN*)
		osname=CYGWIN
		;;
	MINGW*)
		C_FLAGS="-mms-bitfields -mno-cygwin $C_FLAGS"
		osname=MINGW
		;;
	*)
		osname=`echo $osname|tr "a-z" "A-Z"`
		;;
esac
fi
##################
##################oneof()
##################{
##################	for i in $* ; do
##################		if [ -f $i ] ; then
##################			echo $i
####################################			return 0
##################		fi
##################	done
##################}

##################readans()
##################{
##################	local ans
##################	if [ -z "$yesf" ] ; then
##################		printf  "$1 [$2]: " >&2
##################		read ans <&2
##################	else
##################		echo "$1 [$2]:" >&2
##################	fi
##################	if [ -n "$ans" ] ; then
##################		echo $ans
##################	else
##################		echo $2
##################	fi
##################}


DLLIB=-ldl
ADDLIBS=""
case "$osname" in
OPENBSD)
	NM_UNDERSCORE=yes
	DLLIB=""
	;;
*BSD)
	#	BSD-os's have dl support in libc
	#
	DLLIB=""
	USE_AS=yes
	;;
CYGWIN)
	DLLIB=""
	DLLSUFF=".dll.a"
	DLLREALSUFF=".dll"
	EXESUFF=$EXESUFF.exe
	CLIP_DLLIMPORT="__declspec(dllimport)"
	CLIP_DLLEXPORT="__declspec(dllexport)"
	NM_UNDERSCORE=yes
	ADDLIBS="-lregex"
	USE_AS=yes
	;;
MINGW)
	CC="gcc"
	LD_PRG="gcc -shared"
	LD_END=""
	LDS_PRG="gcc -static"
	LDS_END=""
	USE_LD=yes
	DLLIB="-lltdl"
	DLLSUFF=".dll.a"
	DLLREALSUFF=".dll"
	EXESUFF=$EXESUFF.exe
	CLIP_DLLIMPORT="__declspec(dllimport)"
	CLIP_DLLEXPORT="__declspec(dllexport)"
	NM_UNDERSCORE=yes
	ADDLIBS="-lregex -lltdl -lwsock32"
	USE_AS=yes
	;;
*SUNOS)
	NM_PRG="nm -p"
	ADDLIBS="-lsocket -lnsl"
	STD_LIB_DIR=/usr/local/lib
	FORCEALIGN=4
	;;
LINUX)
	if [[ $C64 = "64" ]] ; then
		LD_PRG="ld -A x86_64 -Bdynamic --dynamic-linker --libexecdir=/usr/lib$C64 -L/usr/lib$C64 /lib$C64/ld-linux-x86_64.so.2 /usr/lib$C64/crt1.o /usr/lib$C64/crti.o"
	else
		LD_PRG="ld -A i586 -Bdynamic --dynamic-linker -L/usr/lib$C64 /lib$C64/ld-linux.so.2 /usr/lib$C64/crt1.o /usr/lib$C64/crti.o"
	fi
	LD_END="-L/usr/lib$C64 /usr/lib$C64/crtn.o -lc"
	ADDLIBS="-L/usr/lib$C64 -ldl -lm"
	LDS_PRG="ld -Bstatic -L/usr/lib$C64 /usr/lib$C64/crt1.o /usr/lib$C64/crti.o"
	LDS_END="-L/usr/lib$C64 /usr/lib$C64/crtn.o -lc -lc_pic"

	USE_LD=yes
	USE_AS=yes
	Check_Include gpm.h xyz clip Yes /usr/include "libgpm-devel-xx-$ARCH.(deb/rpm)"
	if [ -f /usr/include/gpm.h ] ; then
		ADDLIBS="$ADDLIBS -lgpm"
	fi
	;;
*)
	;;
esac



if [ -f ./Makefile.in ] ; then
	#	read cashed values
	#
	if [ -f ./clipcfg.sh ] ; then
		source tools/clipcfg.sh
	fi
fi
Check_Bin flex 	xyz 	clip 			Yes 	/usr/bin 			"gettext-devel-xx-$ARCH.(rpm/deb)"
flex -V >/dev/null 2>&1
if [ $? != 0 ] ; then
	echo "Warning: FLEX package does not installed"
	echo "Warning: clip compilation may have a problems"
	#exit 1
fi
Check_Bin bison 	xyz 	clip 			Yes 	/usr/bin 			"gettext-devel-xx-$ARCH.(rpm/deb)"
bison -V >/dev/null 2>&1
if [ $? != 0 ] ; then
	echo "Warning: BISON package does not installed"
	echo "Warning: clip compilation may have a problems"
	#exit 1
fi

#cd xclip
#	if xmkmf >/dev/null 2>&1
#	then
#		if make xclip >/dev/null 2>&1
#		then
#			XCLIP=xclip/xclip
#		fi
#	fi
#cd ..

echo "configure: Makefile creating.." >&0

exec 																										3>Makefile.incl

#CLIP=clip$EXESUFF
CLIP=$Clip_B_Dir/clip$EXESUFF

echo "export OSNAME=$osname" 																		>&3
echo "export DLLIB=$DLLIB" 																		>&3
echo "export ARCH=$arch"  																			>&3
echo "export C64=$C64"  																			>&3
echo "export ADD_CFLAGS=\"$ADD_CFLAGS\""  													>&3
echo "export CLIP=$CLIP"  																			>&3
echo "export DLLSUFF=$DLLSUFF"  																	>&3
echo "export DESTDIR=$DESTDIR"																	>&3
echo "export EXESUFF=$EXESUFF" 																	>&3
echo "export DLLREALSUFF=$DLLREALSUFF" 														>&3
echo "export Makefile_end_in=$Clip_M_Dir/init/Makefile.end.in"							>&3
echo "export Makefile_o_in=$Clip_M_Dir/init/Makefile.o.in" 								>&3
echo "export Makefile_02_in=$Clip_M_Dir/init/Makefile.02.in" 							>&3
echo "export Makefile_end_in=$Clip_M_Dir/init/Makefile.end.in"							>&3
echo "export CONFIGURE_SH=$Clip_M_Dir/init/configure.sh"									>&3
echo "export Clip_M_Dir=$Clip_M_Dir"															>&3
#echo "Clip_H_Dir=$Clip_H_Dir"																	>&3
if [ -n "$FORCEALIGN" ] ; then
	echo "export FORCEALIGN=$FORCEALIGN"														>&3
fi									

echo "MLIB=$MLIB"																						>&3
if [ -n "$MDBG" ] ; then
	echo "export MALLOC_H=memdebug/malloc.h"													>&3
	echo export USE_MEMDEBUG=yes																	>&3
	echo export INST_MEMDEBUG_H=inst_memdebug_h												>&3
fi

echo "export NM_UNDERSCORE=$NM_UNDERSCORE" 													>&3
echo "export STATICLINK=$STATICLINK"															>&3
echo "export CLIP_ROOT=$DESTDIR$CLIPROOT"														>&3
echo "export CLIPROOT=$CLIPROOT"																	>&3
echo "export BINDIR=$BINDIR"																		>&3
echo "export INSTDIR=$CLIPROOT"																	>&3
#echo "export CLIPFLAGS=-w "																		>&3
#echo "export DLLSUFF=$DLLSUFF" 																	>&3
#echo "export EXESUFF=$EXESUFF"																	>&3
echo "export SCRIPTSUFF=$SCRIPTSUFF"															>&3
#echo "export DLLREALSUFF=$DLLREALSUFF"														>&3
echo "export OS_$osname=yes"																		>&3
echo "export CLIPFLAGS=\"$CLIPFLAGS\""															>&3
echo "export C_FLAGS=\"$C_FLAGS\""																>&3
echo "export C_LIBS=\"$C_LIBS\""																	>&3
Check_Include readline/readline.h xyz clip yes /usr/include "libreadline-devel-xx-$ARCH.(rpm/deb)" "tclreadline-xx-$ARCH.(rpm/deb)"
if [ -f /usr/include/readline/readline.h ] ; then
	case "$osname" in
	FREEBSD*)
		READLINE_LIBS="-lreadline -lncurses"
		;;
	NETBSD*)
		READLINE_LIBS=""
		;;
	LINUX*)
		READLINE_LIBS="-lreadline -lncurses"
		;;
	*SUNOS)
		READLINE_LIBS="-lreadline -lcurses"
		;;
	*)
		READLINE_LIBS="-lreadline"
		;;
	esac
	echo "export READLINE_LIBS=\"$READLINE_LIBS\""											>&3
fi

#if [ "$USE_TASKS" = yes -a ! -f libcliptask/task.o ] ; then
#	cd libcliptask
#	./cl_conf
	#cat ../Makefile.incl >Makefile
	#cat Makefile.in >>Makefile
	#$MAKE test_pth
#	./configure
#	if [ $? = 0 ] ; 	then
		USE_TASKS=yes
#	else
#		USE_TASKS=no
#		echo "Warning: task not available: unknown system"
#	fi
#	cd ..
#fi

#if [ "$USE_TASKS" = yes -a -f libcliptask/USE_PTH ] ; then
    ADDLIBS="$ADDLIBS -lpth"
#fi

#if [ "$USE_TASKS" = yes ] ; then
	echo "export USE_TASKS=yes"																	>&3
	echo "export TASK=libcliptask/libcliptask.a"												>&3
#fi

echo "export STD_LIBDIR=\"$STD_LIB_DIR\""														>&3
echo "export NO_GETTEXT=$NO_GETTEXT" 															>&3
Check_Bin gcc xyz 	clip 			Yes 	/usr/bin 			"gettext-devel-xx-$ARCH.(rpm/deb)"

if [ -z "$CC" ] ; then
	if [ -x /usr/bin/gcc -o -x /usr/local/bin/gcc ] ; 	then
		CC=gcc
	else
		CC=cc
	fi
fi
echo "export CC=$CC"																					>&3

test -n "$AS_PRG" || AS_PRG=as
echo "export AS_PRG=$AS_PRG"																		>&3

#test -n "$LD_PRG" || LD_PRG=ld
echo "export LD_PRG=\"$LD_PRG\"" 																>&3
echo "export LD_END=\"$LD_END\""																	>&3
echo "export LDS_PRG=\"$LDS_PRG\""																>&3
echo "export LDS_END=\"$LDS_END\""																>&3

test -n "$NM_PRG" || NM_PRG=nm
echo "export NM_PRG=$NM_PRG"																		>&3


if [ -n "$STD_LIBDIR" ] ; then
	echo "export INST_STD_LIB=inst_std_lib"													>&3
	echo "export STD_LIBDIR=$STD_LIB_DIR"														>&3
fi									

#if [ -n "$XCLIP" ] ; then
#	echo "INST_XCLIP=inst_xclip" 																	>&3
	#	echo "XCLIP=$XCLIP"																			>&3
#fi
PO_BINS="po_extr$EXESUFF po_subst$EXESUFF po_compat$EXESUFF"

echo '
#include </opt/iconv/include/iconv.h>
int main(int argc, char **argv) { iconv_t it; it = iconv_open("utf-8", "utf-8"); return 0;}
' > /tmp/$$.c
$CC --verbose -o /tmp/$$ /tmp/$$.c -L/opt/liconv/lib -liconv  2>/dev/null 1>&2
if [ $? = 0 ] ; then
	ICONV_LIB="-L/opt/iconv/lib -liconv"
	ICONV_INC="/opt/iconv/include/iconv.h"
	HAVE_ICONV=yes
else
	echo '
	#include </usr/local/include/iconv.h>
	int main(int argc, char **argv) { iconv_t it; it = iconv_open("utf-8", "utf-8"); return 0;}
	' > /tmp/$$.c
	$CC --verbose -o /tmp/$$ /tmp/$$.c -L/usr/local/lib -liconv  2>/dev/null 1>&2
	if [ $? = 0 ] ; 	then
		ICONV_LIB="-L/usr/local/lib -liconv"
		ICONV_INC="/usr/local/include/iconv.h"
		HAVE_ICONV=yes
	else
		echo '
		#include <iconv.h>"
		int main(int argc, char **argv) { iconv_t it; it = iconv_open("utf-8", "utf-8"); return 0;}
		' > /tmp/$$.c
		$CC -o /tmp/$$ /tmp/$$.c 2>/dev/null 1>&2
		if [ $? = 0 ] ; then
			ICONV_LIB=""
			ICONV_INC="<iconv.h>"
			HAVE_ICONV=yes
		else
			ICONV_INC="<iconv.h>"
			ICONV_LIB=""
			echo "NO ICONV library found, doc building will have problems"
			PO_BINS="po_extr$EXESUFF po_compat$EXESUFF"
			HAVE_ICONV=
		fi
	fi
fi
echo "#include $ICONV_INC" >include.h/cl_iconv.h

echo "export ICONV_INC=$ICONV_INC"   															>&3
echo "export ICONV_LIB=\"$ICONV_LIB\""															>&3
echo "export PO_BINS=\"$PO_BINS\""																>&3
echo "export CFLAGS=\"$C_FLAGS\""																>&3
echo "ICONV_INC=$ICONV_INC" >&0
echo "ICONV_LIB=$ICONV_LIB" >&0

rm -f /tmp/$$*


echo																										>&3
Check_Bin msgfmt 	xyz 	clip 			Yes 	/usr/bin 			"gettext-devel-xx-$ARCH.(rpm/deb)"
msgfmt -V >/dev/null 2>&1
if [ $? != 0 ] ;	then
	echo "Warning: GETTEXT package does not installed"
	echo "Warning: clip_msgfmt and clip_msgmerge will not work"
	NO_GETTEXT=yes
fi

echo '
msgid "error"
msgid_plural "errors"
msgstr[0] ""
msgstr[1] ""
'> /tmp/$$msg

if msgfmt -o /dev/null /tmp/$$msg >/dev/null 2>&1 ; then
	PO_FROM_COMPAT="cat"
	PO_TO_COMPAT="cat"
	echo "export PO_COMPAT="																		>&3
else
	PO_COMPAT=yes
	PO_FROM_COMPAT="$CLIPROOT/bin/po_compat -f"
	PO_TO_COMPAT="$CLIPROOT/bin/po_compat -t"
	echo export PO_COMPAT=yes																		>&3
fi
ADDLIBS="$ADDLIBS $ICONV_LIB $READLINE_LIBS"
echo "export PO_FROM_COMPAT=\"$PO_FROM_COMPAT\"" 											>&3
echo "export PO_TO_COMPAT=\"$PO_TO_COMPAT\"" 												>&3

echo "export ADDLIBS=\"$ADDLIBS\""																>&3
#echo "export "																						>&3

echo 																										>&3

rm -f /tmp/$$msg

exec																										3>&-
cat ../configure.ini >>Makefile.incl

#sort -u -d -t = <Makefile.incl -o Makefile.dtu
sort -d -t =  <Makefile.incl -o Makefile.dtx

#tr -d \' <./Makefile.dtu >./Makefile.inc
tr -d \' <./Makefile.dtx >./Makefile.inc
tr -d \" <./Makefile.inc >./Makefile.tmp

echo "#	Created automatically by \"configure\"" 	>./Makefile.inc
echo "#" 														>>./Makefile.inc
echo  															>>./Makefile.inc
echo "#" 														>>./Makefile.inc
echo "#	Compile flags" 									>>./Makefile.inc
echo "#" 														>>./Makefile.inc
cat ./Makefile.tmp 											>>./Makefile.inc
echo "include \$(Makefile_02_in)" 						>>./Makefile.inc

echo "#!/bin/bash" 											>./clipcfg.sh
echo "#	Created automatically by \"configure\"" 	>>./clipcfg.sh
echo "#" 														>>./clipcfg.sh
echo 																>>./clipcfg.sh
echo "#" 														>>./clipcfg.sh
echo "#	Compile flags" 									>>./clipcfg.sh
echo "#" 														>>./clipcfg.sh
cat Makefile.incl 											>>./clipcfg.sh

echo export `cat Makefile.inc | grep "^[A-Z_][A-Z_]*=" | cut -d"=" -f1` > Makefile.export


cat Makefile.export 										>>clipcfg.sh
cat Makefile.export 										>>Makefile.inc

#cat ./Makefile.inc 				>Makefile
#cat ./Makefile.in 				>>Makefile

cp -f$V Makefile.inc ../Makefile.ini
#cp -f$V Makefile.ini ../
mkdir -p$V $DESTDIR$CLIPROOT/include
cp -f$V ../Makefile.ini $DESTDIR$CLIPROOT/include/Makefile.ini

rm -f$V Makefile.incl Makefile.inc Makefile.dtu

echo ". done."


printf "configure: creating clipcfg.h .."

exec																		3>clipcfg.h

echo "/* Created automatically by \"configure\" */"			>&3
echo "#ifndef CLIP_CONFIG_H"										>&3
echo "#define CLIP_CONFIG_H"										>&3
echo																		>&3
exec 																		3>&-

exec																													3>temp$$1$$2
echo "#define CLIP_VERSION \"$(cat $Clip_M_Dir/config/release_version)\""						>&3
echo "#define DLLIB \"$DLLIB\""																				>&3
echo "#define ADDLIBS \"$ADDLIBS\""																			>&3
echo "#define ADD_CFLAGS \"$ADD_CFLAGS\"" 																>&3
echo "#define DLLSUFF \"$DLLSUFF\""																			>&3
echo "#define EXESUFF \"$EXESUFF\""																			>&3
echo "#define DLLREALSUFF \"$DLLREALSUFF\""																>&3
echo "#define CLIP_DLLEXPORT $CLIP_DLLEXPORT"															>&3
echo "#define CLIP_DLLIMPORT $CLIP_DLLIMPORT"															>&3
echo "#define NM_PRG \"$NM_PRG\""										 									>&3
USE_AS=no
if [ "$USE_AS" = no ] ; then
	echo "#undef USE_AS"																							>&3
	echo "#define AS_PRG 0"																						>&3
else
	echo "#define USE_AS"																						>&3
	echo "#define AS_PRG \"$AS_PRG\""																		>&3
fi
if [ "$USE_LD" = yes ] ; then
	echo "#define USE_LD" 																						>&3
	echo "#define LD_PRG \"$LD_PRG\"" 																		>&3
	echo "#define LD_END \"$LD_END\"" 																		>&3
	echo "#define LDS_PRG \"$LDS_PRG\""																		>&3
	echo "#define LDS_END \"$LDS_END\""																		>&3
else
	echo "#undef USE_LD" 																						>&3
	echo "#define LD_PRG 0"																						>&3
fi
echo "#define OSNAME \"$osname\""																			>&3
echo "#define ARCH \"$arch\"" 																				>&3
echo "#define ARCH_$arch"																						>&3
echo "#define OS_$osname" 																						>&3
echo "#define SYSTEM \"$uname\"" 																			>&3
echo "#define CLIP_ROOT \"$DESTDIR$CLIPROOT\"" 															>&3
echo "#define BINDIR \"$BINDIR\"" 																			>&3

if [ "$CHECK_STACK" = yes ] ; then
	echo "#define WITH_CHECK_STACK  1" 																		>&3
else
	echo "#undef WITH_CHECK_STACK"																			>&3
fi

#if [ -n "$MEMDBGFLAGS" ]
#then
#	echo "#define MEMDBGFLAGS \"$MEMDBGFLAGS\""															>&3
#fi

if [ -n "$MDBG" ] ; then
#	echo "#define MEMDEBUG \"`memdebug/malloc_dbg -f memdebug/mallocrc $MEMDEBUG 2>&-` \"">&3
	dvar=`memdebug/malloc_dbg -f memdebug/mallocrc -b ${MEMDEBUG} 2>&-`
	echo "#define MEMDEBUG \"	`eval $dvar; echo $MALLOC_DEBUG `\""								>&3
	echo "#define MALLOC_FUNC_CHECK"																			>&3
fi
#Check_Include float.h xyz clip yes
if [ -f /usr/include/float.h ] ; then
	echo "#define HAVE_FLOAT_H"																				>&3
fi

if [ -f /usr/include/sys/io.h ] ; then
	echo "#define HAVE_SYS_IO_H"																				>&3
fi

if [ -f /usr/bin/wget ] ; then
	echo "#define WGET_PRG \"/usr/bin/wget\""																>&3
else
	if [ -f /usr/local/bin/wget ] ; 	then
		echo "#define WGET_PRG \"/usr/local/bin\""															>&3
	else
		echo "Warning: system does not have program WGET"
	fi
fi

if [ -f /usr/include/zlib.h ] ; then
	echo "#define HAVE_ZLIB_H"																					>&3
else
	echo "Warning: system do not have installed ZLIB development"
fi
if [ -f /usr/include/regex.h ] ; then
	echo "#define HAVE_REGEX_H"																				>&3
else
	echo "Warning: system do not have installed REGEX development"
fi
if [ -f /usr/include/readline/readline.h ] ; then
	echo "#define HAVE_READLINE_H"																			>&3
	echo '
#include <stdio.h>
#include <readline/readline.h>
int main(int argc, char **argv) { rl_already_prompted=1; return 0;}
	' > /tmp/$$.c
	$CC -o /tmp/$$ /tmp/$$.c $READLINE_LIBS 2>/dev/null 1>&2
	if [ $? = 0 ] ; 	then
		echo "#define HAVE_READLINE_ALREADY_PROMPTED" 													>&3
	else
		echo "Warning: old READLINE version"
	fi
	rm -f /tmp/$$*
else
	echo "Warning: system does not have READLINE development"
	echo "Warning: clip_dbg will not work !!!"
fi

if [ "$osname" = "SUNOS" ]
then
	echo "#undef USE_UTMP"																						>&3
else

echo '
#include <utmp.h>
int main(int argc, char **argv) { setutent(); return 0;}
	' > /tmp/$$.c
$CC -o /tmp/$$ /tmp/$$.c 2>/dev/null 1>&2
if [ $? = 0 ] ; then
	echo "#define USE_UTMP 1" 																					>&3
else
	echo "#undef USE_UTMP"																						>&3
fi
rm -f /tmp/$$*

fi

echo '
#include <sys/time.h>
int main(int argc, char **argv) { struct timeval tv; gettimeofday(&tv, 0); return 0;}
	' > /tmp/$$.c
$CC -o /tmp/$$ /tmp/$$.c 2>/dev/null 1>&2
if [ $? = 0 ]
then
	echo "#define HAVE_GETTIMEOFDAY 1"																		>&3
else
	echo "#undef HAVE_GETTIMEOFDAY"																			>&3
fi
rm -f /tmp/$$*


if [ -f /usr/include/sys/mman.h ]
then
	echo "#define HAVE_MMAN_H"																					>&3
else
	echo "#define NO_MMAP"																						>&3
fi

if [ -f /usr/include/sys/vfs.h ]
then
	echo "#define HAVE_VFS_H"																					>&3
fi

if [ -f /usr/include/sys/statvfs.h ]
then
	echo "#define HAVE_STATVFS_H"																				>&3
fi

#	ptmx support
#
if [ -c /dev/ptmx -a -f /usr/include/stropts.h ]
then
	echo "#define USE_PTS"																						>&3
fi

if [ -n "$STD_LIBDIR" ]
then
	echo "#define STD_LIBDIR \"$STD_LIB_DIR\""															>&3
fi

if [ -n "$USE_WCHARS" ]
then
	echo "#define USE_WCHARS"																					>&3
fi

if [ -n "$NM_UNDERSCORE" ]
then
	echo "#define NM_UNDERSCORE"																				>&3
fi

if [ "$USE_TASKS" = yes ]
then
	echo "#define USE_TASKS 1"																					>&3
else
	echo "#undef USE_TASKS"																						>&3
fi

if [ -n "$FORCEALIGN" ]
then
	echo "#define FORCEALIGN $FORCEALIGN"																	>&3
fi

if [ -f /usr/include/gpm.h ]
then
	echo "#define HAVE_GPM_H"																					>&3
fi

if [ "$PO_COMPAT" = yes ]
then
	echo "#define PO_COMPAT 1"																					>&3
	echo "#define PO_COMPAT_CHAR 0177"																		>&3
else
	echo "#undef PO_COMPAT"																						>&3
fi

if [ "$HAVE_ICONV" = yes ]
then
	echo "#define HAVE_ICONV 1"																				>&3
	echo "#include \"$ICONV_INC\"" >$DESTDIR$CLIPROOT/include/cl_iconv.h
else
	echo "#undef HAVE_ICONV"																					>&3
fi

case "$osname" in
	CYGWIN*)
		echo "#define _WIN32"																					>&3
		;;
	MINGW*)
		echo
		echo "#define random rand"																				>&3
		echo "#define srandom srand" 																			>&3
		echo "#define ftruncate chsize"																		>&3
		echo
		#echo "#define _WIN32"																					>&3
		;;
esac

exec 																													3>&-
#exec 3>./temp$$1$$2
sort -u -d <temp$$1$$2 -o temp$$1$$2.out

cat temp$$1$$2.out >>clipcfg.h
echo >>clipcfg.h
echo "#endif" >>clipcfg.h
echo ". done."
cp -fup$V clipcfg.h $Clip_I_Dir/
rm -f$V temp$$1$$2 temp$$1$$2.out

cd $pwd/tools
#source clip_msgmerge.in.sh
#source clip_msgfmt.in.sh
#source clip_makelib.in.sh
#source clip_makeslib.in.sh
#source clip_cp.in.sh

reg=1
if [ -f registration ]
then
	./rsa-verify >license.txt <registration
	reg=$?
	echo >>license.txt
	echo "REGISTRATION BEGIN" >>license.txt
	cat registration >>license.txt
	echo "REGISTRATION END" >>license.txt
fi

if [ "$reg" != 0 ]
then
	cp license.gnu license.txt
fi

echo "Registration info:"
echo

cat license.txt
echo

exec 																			3>license.h

echo '#define CLIP_LICENSE "\' 										>&3
#sed -e 's/"/\\"/g' -e 's/^.*$/"\0\\\n"\\/g' <license.txt 	>&3
sed -e 's/"/\\"/g' -e 's/^.*$/\0\\n\\/g' <license.txt			>&3
echo '"'																		>&3
rm -f license.txt

exec																			3>&-
echo ". done."

mkdir -p$V $DESTDIR$CLIPROOT/cliprc/
echo "-v0
-O
-r
-l" > $DESTDIR$CLIPROOT/cliprc/clipflags

#cd $Clip_M_Dir
#cat Makefile.ini >Makefile
#cat Makefile.in >>Makefile