#!/bin/sh
#
#	Configure script for xDevil by Angelo GIRARDI 2009-11-14
# 	adapted from clip-itk
#	Paul Lasarev - 2000.
#
$xDevil_M_Dir/init/create.dir.sh
source $xDevil_M_Dir/init/functions.f.sh
if [[ $? != 0 ]] ; then
	exit 1
fi
cd "$xDevil_M_Dir"
source $xDevil_M_Dir/init/check.packages.sh
if [[ $? != 0 ]] ; then
	exit 1
fi
INSTALL_ROOT="false"
if [[ "$option" = "sys" ]] || [[ "$option" = "opt" ]] || [[ "$option" = "usr.local" ]] ; then
	INSTALL_ROOT="true"
fi
xDevil_CMDSTR="-w,-l,-N"
FORCEALIGN=""
MLIB=""
MDBG=""
osname="LINUX"
Std_include="-I /usr/include"
OPTFLAGS=""
STATICLINK=""
NO_GETTEXT=""
CC=""
AS_PRG=""
NM_PRG=""
CHECK_STACK=""
PO_COMPAT=""
#SCRIPTSUFF=$C64.sh
DEBUGFLAGS="-DxDevil_DEBUG"
DEBUGFLAGS="-DDEBUGGING_xDevil"
#DEBUGFLAGS=""
STD_LIBDIR=""
if [[ "$C64" == "64" ]] ; then
	STD_LIB_DIR=/lib64:/usr/lib$C64:/usr/lib$C64:/usr/lib
else
STD_LIB_DIR=/lib:/usr/lib$C64:/usr/lib
fi
STD_LIB_DIR=/usr/lib$C64
DLLSUFF=".so"
DLLREALSUFF=".so"
CLIP_DLLIMPORT=""
CLIP_DLLEXPORT=""
NM_UNDERSCORE=""
#EXESUFF=$C64
USE_WCHARS=yes
USE_TASKS=yes
LibDir=lib$C64
YACC="bison"
#if [ "$xDevil_TASKS" ]
#then
#	USE_TASKS=$xDevil_TASKS
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
rm -f$V Makefile Makefile.inc xDevilcfg.sh
USE_MEMDBG=no
MEMDEBUG_LEVEL=0
MEMDBGFLAGS=
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
[ -z "$xDevilROOT" ] && xDevilROOT=`cd ../..;pwd`/xDevilroot
BINDIR="$HOME/bin"
[ -d "$BINDIR" ] || mkdir -p$V "$BINDIR"
xDevil_ROOT="$INSTDIR"
xDevilROOT="$INSTDIR"
#	initial C_FLAGS
#
#C_FLAGS="-Wall -I. $DEBUGFLAGS $OPTFLAGS $MDBG"
C_FLAGS="-Wall -fPIC -I. -I include $Std_include $DEBUGFLAGS $OPTFLAGS"
#C_FLAGS="-Wall -fPIC"
xDevilFLAGS="-I. -I include -wlON $Std_include $DEBUGFLAGS  $OPTFLAGS"
C_LIBS="-L$xDevil_L_Dir -lxDevil"
ADD_CFLAGS="-fPIC"
uname=`uname -s`
uver=`uname -r`
##seq_no=$(cat $xDevil_M_Dir/release.version)
####remark from Uri: "hostname -f" on Solaris changed hostname !
####                 and "hname" not used in this script.
#hname=`hostname -f 2>/dev/null`
#if [ $? != 0 ]
#then
#	hname=`hostname 2>/dev/null`
#	if [ $? != 0 ]
#	then
#		hname=`uname -n 2>/dev/null`
#		if [ $? != 0 ]
#		then
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
		ADDLIBS="-L/lib$C64 -L/usr/lib$C64 -L/usr/local/lib$C64 -L/lib -L/usr/lib -L/usr/local/lib -ldl -lm"
	else
		LD_PRG="ld -A i586 -Bdynamic --dynamic-linker -L/usr/lib$C64 /lib$C64/ld-linux.so.2 /usr/lib$C64/crt1.o /usr/lib$C64/crti.o"
		ADDLIBS="-L/lib$C64 -L/usr/lib$C64 -L/usr/local/lib$C64 -ldl -lm"
	fi
	LD_END="-L/usr/lib$C64 /usr/lib$C64/crtn.o -lc"
	LDS_PRG="ld -Bstatic -L/usr/lib$C64 /usr/lib$C64/crt1.o /usr/lib$C64/crti.o"
	LDS_END="-L/usr/lib$C64 /usr/lib$C64/crtn.o -lc -lc_pic"
	USE_LD=yes
	USE_AS=yes
	if [ -f /usr/include/gpm.h ] ; then
		ADDLIBS="$ADDLIBS -lgpm"
	fi
	;;
*)
	;;
esac
msgfmt -V >/dev/null 2>&1
if [ $? != 0 ] ; then
	echo "Warning: GETTEXT package does not installed"
	echo "Warning: xDevil_msgfmt and xDevil_msgmerge will not work"
	NO_GETTEXT=yes
fi
flex -V >/dev/null 2>&1
if [ $? != 0 ] ; then
	echo "Warning: FLEX package does not installed"
	echo "Warning: xDevil compilation may have a problems"
	#exit 1
fi
bison -V >/dev/null 2>&1
if [ $? != 0 ] ; then
	echo "Warning: BISON package does not installed"
	echo "Warning: xDevil compilation may have a problems"
	#exit 1
fi
echo "configure: Makefile creating.." >&0
exec 																												3>Makefile.incl
xDevil=$xDevil_B_Dir/xDevil$EXESUFF
xDevil_CP=$xDevil_B_Dir/xDevil_cp$SCRIPTSUFF
xDevil_MAKELIB=$xDevil_B_Dir/xDevil_makelib$SCRIPTSUFF
xDevil_MAKESLIB=$xDevil_B_Dir/xDevil_makeslib$SCRIPTSUFF
xDevil_MSGFMT=$xDevil_B_Dir/xDevil_msgfmt$SCRIPTSUFF
xDevil_MSGMERGE=$xDevil_B_Dir/xDevil_msgmerge$SCRIPTSUFF
OSNAME=$osname
Makefile_end_in=$xDevil_M_Dir/init/Makefile.end.in
Makefile_o_in=$xDevil_M_Dir/init/Makefile.o.in
Makefile_01_in=$xDevil_M_Dir/init/Makefile.01.in
Makefile_02_in=$xDevil_M_Dir/init/Makefile.02.in
Makefile_gen_in=$xDevil_M_Dir/init/Makefile.gen.in
CONFIGURE_SH=/$xDevil_M_Dir/init/configure.sh

if [ -n "$FORCEALIGN" ] ; then
	echo "export FORCEALIGN=$FORCEALIGN"																>&3
fi
if [ -n "$MDBG" ] ; then
	echo "export MALLOC_H=memdebug/malloc.h"															>&3
	echo "export USE_MEMDEBUG=yes"																		>&3
	echo "export INST_MEMDEBUG_H=inst_memdebug_h"													>&3
fi
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
fi

CurDir="$PWD"
ADDLIBS="$ADDLIBS -lpth"
if [ "$USE_TASKS" = yes ] ; then
	echo "export USE_TASKS=yes"																			>&3
	echo "export TASK=libxDeviltask/libxDeviltask.a"														>&3
fi
echo "export NO_GETTEXT=$NO_GETTEXT" 																	>&3
if [ -z "$CC" ] ; then
	if [ -x /usr/bin/gcc -o -x /usr/local/bin/gcc ] ; then
		CC=gcc
	else
		CC=cc
	fi
fi
[ $CC -nt ./configure.gen.sh ] && touch ./configure.gen.sh
if [ -n "$STD_LIBDIR" ] ; then
	echo "export INST_STD_LIB=$INST_STD_LIB"															>&3
	echo "export STD_LIBDIR=$STD_LIB_DIR"																>&3
fi
#if [ -n "$XCLIP" ] ; then
#	echo "export INST_XCLIP=inst_xclip" 																>&3
	#	echo "export XCLIP=$XCLIP"																			>&3
#fi
PO_BINS="po_extr$EXESUFF po_subst$EXESUFF po_compat$EXESUFF"
$CC -o /tmp/$$ $xDevil_M_Dir/external/test/test.iconv.opt.c -liconv  -L/opt/liconv/lib
if [ $? = 0 ] ; then
	ICONV_LIB="-liconv  -L/opt/iconv/lib"
	ICONV_INC="\"/opt/iconv/include/iconv.h\""
	LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/opt/iconv/lib"
	HAVE_ICONV=yes
else
	echo "Error of NO IMPORTANCE" >&0
	echo "Error of NO IMPORTANCE" >&1
	echo "Error of NO IMPORTANCE" >&2
	$CC -o /tmp/$$ $xDevil_M_Dir/external/test/test.iconv.local.c -liconv -L/usr/local/lib
	if [ $? = 0 ] ; 	then
		ICONV_LIB="-liconv  -L/usr/local/lib"
		ICONV_INC="\"/usr/local/include/iconv.h\""
		LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/usr/local/lib"
		HAVE_ICONV=yes
	else
		$CC -o /tmp/$$ $xDevil_M_Dir/external/test/test.iconv.c
		if [ $? = 0 ] ; then
			ICONV_LIB=""
			ICONV_INC="<iconv.h>"
			HAVE_ICONV=yes
		else
			ICONV_INC="<iconv.h>"
			ICONV_LIB=""
			echo "NO ICONV library found, doc building will have problems"
			echo "NO ICONV library found, doc building will have problems" >&1
			echo "NO ICONV library found, doc building will have problems" >&2
			PO_BINS="po_extr$EXESUFF po_compat$EXESUFF"
			HAVE_ICONV=
		fi
	fi
fi
[ -d $xDevil_I_Dir ] || mkdir -p$V $xDevil_I_Dir
echo "#include $ICONV_INC" >$xDevil_I_Dir/cl_iconv.h

test -n "$AS_PRG" || AS_PRG=as
test -n "$NM_PRG" || NM_PRG=nm


echo "ICONV_INC=$ICONV_INC" >&0
echo "ICONV_LIB=$ICONV_LIB" >&0
echo "ICONV_INC=$ICONV_INC" >&1
echo "ICONV_LIB=$ICONV_LIB" >&1
echo "ICONV_INC=$ICONV_INC" >&2
echo "ICONV_LIB=$ICONV_LIB" >&2
rm -f$V /tmp/$$*
echo '
msgid "error"
msgid_plural "errors"
msgstr[0] ""
msgstr[1] ""
'> /tmp/$$msg
if msgfmt -o /dev/null /tmp/$$msg >/dev/null 2>&1 ; then
	PO_FROM_COMPAT="cat"
	PO_TO_COMPAT="cat"
	PO_COMPAT=no
else
	PO_COMPAT=yes
	PO_FROM_COMPAT="$xDevil_B_Dir/po_compat$EXESUFF -f"
	PO_TO_COMPAT="$xDevil_B_Dir/po_compat$EXESUFF -t"
fi
ADDLIBS="$ADDLIBS $ICONV_LIB $READLINE_LIBS"
rm -f /tmp/$$msg
echo 																												>&3
echo "export ADD_CFLAGS=\"$ADD_CFLAGS\""  															>&3
echo "export ADDLIBS=\"$ADDLIBS\""																		>&3
echo "export AS_PRG=$AS_PRG"																				>&3
echo "export ARCH=$arch"  																					>&3
echo "export BINDIR=$BINDIR"																				>&3
echo "export C64=$C64"  																					>&3
echo "export CC=$CC"																							>&3
echo "export rV=$rV"																							>&3
echo "export rD=$rD"																							>&3
echo "export C_FLAGS=\"$C_FLAGS\""																		>&3
echo "export C_LIBS=\"$C_LIBS\""																			>&3
echo "export xDevil=$xDevil"  																				>&3
echo "export xDevil_B_Dir=$xDevil_B_Dir"																	>&3
echo "export xDevil_C_Dir=$xDevil_C_Dir"																	>&3
echo "export xDevil_CMDSTR=\"$xDevil_CMDSTR\"" 														>&3
echo "export xDevil_CP=$xDevil_CP"  																			>&3
echo "export xDevil_D_Dir=$xDevil_D_Dir"																	>&3
echo "export xDevilFLAGS=\"$xDevilFLAGS\""																>&3
echo "export xDevil_H_Dir=$xDevil_H_Dir"																	>&3
echo "export xDevil_I_Dir=$xDevil_I_Dir"																	>&3
echo "export xDevil_L_Dir=$xDevil_L_Dir"																	>&3
echo "export xDevil_M_Dir=$xDevil_M_Dir"																	>&3
echo "export xDevil_MAKELIB=$xDevil_MAKELIB"  															>&3
echo "export xDevil_MAKESLIB=$xDevil_MAKESLIB"  															>&3
echo "export xDevil_MSGFMT=$xDevil_MSGFMT"  																>&3
echo "export xDevil_MSGMERGE=$xDevil_MSGMERGE"  															>&3
echo "export xDevil_NAMES=$xDevil_NAMES"  																	>&3
echo "export xDevil_ROOT=$xDevil_ROOT"																		>&3
echo "export xDevilROOT=$xDevilROOT"																			>&3
echo "export xDevil_S_Dir=$xDevil_S_Dir"																	>&3
echo "export xDevil_T_Dir=$xDevil_T_Dir"																	>&3
echo "export CONFIGURE_SH=$CONFIGURE_SH"																>&3
echo "export DLLIB=$DLLIB" 																				>&3
echo "export DLLREALSUFF=$DLLREALSUFF" 																>&3
echo "export DLLSUFF=$DLLSUFF"  																			>&3
echo "export EXESUFF=$EXESUFF" 																			>&3
echo "export INSTALL_ROOT=$INSTALL_ROOT" 																>&3
echo "export INSTDIR=$xDevil_ROOT"																			>&3
echo "export LD_END=\"$LD_END\""																			>&3
echo "export LD_LIBRARY_PATH=$LD_LIBRARY_PATH"														>&3
echo "export LD_PRG=\"$LD_PRG\"" 																		>&3
echo "export LDS_END=\"$LDS_END\""																		>&3
echo "export LDS_PRG=\"$LDS_PRG\""																		>&3
echo "export LibDir=\"$LibDir\""																			>&3
echo "export Makefile_01_in=$Makefile_01_in" 														>&3
echo "export Makefile_02_in=$Makefile_02_in" 														>&3
echo "export Makefile_end_in=$Makefile_end_in"														>&3
echo "export Makefile_gen_in=$Makefile_gen_in"														>&3
echo "export Makefile_o_in=$Makefile_o_in" 															>&3
echo "export Making=$Making"  																			>&3
echo "export MEMDBGFLAGS=\"$MEMDBGFLAGS\""															>&3
echo "export MLIB=$MLIB"																					>&3
echo "export NM_PRG=$NM_PRG"																				>&3
echo "export NM_UNDERSCORE=$NM_UNDERSCORE"															>&3
echo "export PO_COMPAT=$PO_COMPAT"																		>&3
echo "export PO_FROM_COMPAT=$PO_FROM_COMPAT"															>&3
echo "export PO_TO_COMPAT=$PO_TO_COMPAT"																>&3
echo "export READLINE_LIBS=\"$READLINE_LIBS\""														>&3
echo "export OS_$osname=yes"																				>&3
echo "export OSNAME=$osname" 																				>&3
echo "export SCRIPTSUFF=$SCRIPTSUFF"																	>&3
echo "export seq_no=$seq_no"																				>&3
echo "export STATICLINK=$STATICLINK"																	>&3
echo "export STD_LIBDIR=$STD_LIBDIR"																	>&3
echo "export V=$V"  																							>&3
echo "export ICONV_INC=$ICONV_INC"   																	>&3
echo "export ICONV_LIB=\"$ICONV_LIB\""																	>&3
echo "export PO_BINS=\"$PO_BINS\""																		>&3
echo "export PATH=\"$PATH\""																				>&3
echo "export YACC=$YACC"																					>&3
echo "export WaitForCheck=$WaitForCheck"																>&3
echo																												>&3
exec																												3>&-
#sort -u -d -t = <Makefile.incl -o Makefile.dtu
sort -d -t =  <Makefile.incl -o Makefile.incl_1
#tr -d \' <./Makefile.dtu >./Makefile.inc
tr -d \' <./Makefile.incl_1 >./Makefile.incl_2
tr -d \" <./Makefile.incl_2 >./Makefile.incl_3
echo "#	Created automatically by \"configure\"" 	>./Makefile.inc
echo "#" 														>>./Makefile.inc
echo  															>>./Makefile.inc
echo "#" 														>>./Makefile.inc
echo "#	Compile flags" 									>>./Makefile.inc
echo "#" 														>>./Makefile.inc
cat "$Makefile_01_in" 										>>./Makefile.inc
cat ./Makefile.incl_3 										>>./Makefile.inc
#echo "include \$(Makefile_02_in)" 						>>./Makefile.inc
echo "#	Created automatically by \"configure\"" 	>./xDevilcfg.sh
echo "#" 														>>./xDevilcfg.sh
echo 																>>./xDevilcfg.sh
echo "#" 														>>./xDevilcfg.sh
echo "#	Compile flags" 									>>./xDevilcfg.sh
echo "#" 														>>./xDevilcfg.sh
cat Makefile.incl_1 											>>./xDevilcfg.sh
#echo export `cat Makefile.incl_3 | grep "^[A-Z_][A-Z_]*=" | cut -d"=" -f1` > Makefile.export
#cat Makefile.export 										>>xDevilcfg.sh
#cat Makefile.export 										>>Makefile.inc
#cat ./Makefile.inc 				>Makefile
#cat ./Makefile.in 				>>Makefile
cp --remove-destination -fu$V Makefile.inc ./Makefile.ini
mkdir -p$V $xDevil_I_Dir
mv Makefile.ini xDevilcfg.sh $xDevil_I_Dir/
rm -f$V Makefile.dtu
# 		|| true
rm -f$V Makefile.dtx
# 		|| true
rm -f$V Makefile.export
# 	|| true
rm -f$V Makefile.inc
# 		|| true
rm -f$V Makefile.incl*
# 		|| true
rm -f$V Makefile.ini
# 		|| true
rm -f$V Makefile.tmp
# 		|| true
cp --remove-destination -fu$V $xDevil_I_Dir/Makefile.ini $xDevil_M_Dir/
echo ". done."
printf "configure: creating dv_xDevilcfg.h .."
exec																		3>dv_xDevilcfg.h
echo "/* Created automatically by \"configure\" */"		>&3
echo "#ifndef xDevil_CONFIG_H"										>&3
echo "#define xDevil_CONFIG_H"										>&3
echo																		>&3
exec 																		3>&-
exec																													3>temp$$1$$2
echo "#define xDevil_VERSION \"$seq_no\""																	>&3
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
echo "#define xDevil_ROOT \"$INSTDIR\"" 																		>&3
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
if [ -f /usr/include/float.h ] ; then
	echo "#define HAVE_FLOAT_H"																				>&3
fi
if [ -f /usr/include/sys/io.h ] ; then
	echo "#define HAVE_SYS_IO_H"																				>&3
fi
if [ -f /usr/bin/wget ] ; then
	echo "#define WGET_PRG \"/usr/bin/wget\""																>&3
else
	if [ -f /usr/local/bin/wget ] ; then
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
	$CC -o /tmp/$$ /tmp/$$.c $READLINE_LIBS
# 2>/dev/null 1>&2
	if [ $? = 0 ] ; then
		echo "#define HAVE_READLINE_ALREADY_PROMPTED" 													>&3
	else
		echo "Warning: old READLINE version"
	fi
	rm -f /tmp/$$*
else
	echo "Warning: system does not have READLINE development"
	echo "Warning: xDevil_dbg will not work !!!"
fi
if [ "$osname" = "SUNOS" ] ; then
	echo "#undef USE_UTMP"																						>&3
else
	echo '
	#include <utmp.h>
	int main(int argc, char **argv) { setutent(); return 0;}
		' > /tmp/$$.c
	$CC -o /tmp/$$ /tmp/$$.c 2>/dev/null 1>&2
	if [ $? = 0 ] ; then
		echo "#define USE_UTMP 1" 																				>&3
	else
		echo "#undef USE_UTMP"																					>&3
	fi
	rm -f /tmp/$$*
fi
echo '
#include <sys/time.h>
int main(int argc, char **argv) { struct timeval tv; gettimeofday(&tv, 0); return 0;}
	' > /tmp/$$.c
$CC -o /tmp/$$ /tmp/$$.c 2>/dev/null 1>&2
if [ $? = 0 ] ; then
	echo "#define HAVE_GETTIMEOFDAY 1"																		>&3
else
	echo "#undef HAVE_GETTIMEOFDAY"																			>&3
fi
rm -f /tmp/$$*
if [ -f /usr/include/sys/mman.h ] ; then
	echo "#define HAVE_MMAN_H"																					>&3
else
	echo "#define NO_MMAP"																						>&3
fi
if [ -f /usr/include/sys/vfs.h ] ; then
	echo "#define HAVE_VFS_H"																					>&3
fi
if [ -f /usr/include/sys/statvfs.h ] ; then
	echo "#define HAVE_STATVFS_H"																				>&3
fi
#	ptmx support
#
if [ -c /dev/ptmx -a -f /usr/include/stropts.h ] ; then
	echo "#define USE_PTS"																						>&3
fi
if [ -n "$STD_LIBDIR" ] ; then
	echo "#define STD_LIBDIR \"$STD_LIB_DIR\""															>&3
fi
if [ -n "$USE_WCHARS" ] ; then
	echo "#define USE_WCHARS"																					>&3
fi
if [ -n "$NM_UNDERSCORE" ] ; then
	echo "#define NM_UNDERSCORE"																				>&3
fi
if [ "$USE_TASKS" = yes ] ; then
	echo "#define USE_TASKS 1"																					>&3
else
	echo "#undef USE_TASKS"																						>&3
fi
if [ -n "$FORCEALIGN" ] ; then
	echo "#define FORCEALIGN $FORCEALIGN"																	>&3
fi
if [ -f /usr/include/gpm.h ] ; then
	echo "#define HAVE_GPM_H"																					>&3
fi
if [ "$PO_COMPAT" = yes ] ; then
	echo "#define PO_COMPAT 1"																					>&3
	echo "#define PO_COMPAT_CHAR 0177"																		>&3
else
	echo "#undef PO_COMPAT"																						>&3
fi
if [ "$HAVE_ICONV" = yes ] ; then
	echo "#define HAVE_ICONV 1"																				>&3
#	echo "#include \"$ICONV_INC\"" >$xDevilROOT/include/cl_iconv.h
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
sort -d <temp$$1$$2 -o temp$$1$$2.out
cat temp$$1$$2.out >>dv_xDevilcfg.h
echo >>dv_xDevilcfg.h
echo "#endif" >>dv_xDevilcfg.h
echo ". done."
rm -f$V temp$$1$$2 temp$$1$$2.out
mv dv_xDevilcfg.h $xDevil_I_Dir/
cd $xDevil_M_Dir/xDevil/tools
source xDevil_msgmerge.in.sh
if [[ $? != 0 ]] ; then
	error $0 $LINENO
	exit 1
fi
source xDevil_msgfmt.in.sh
if [[ $? != 0 ]] ; then
	error $0 $LINENO
	exit 1
fi
source xDevil_makelib.in.sh
if [[ $? != 0 ]] ; then
	error $0 $LINENO
	exit 1
fi
source xDevil_makeslib.in.sh
if [[ $? != 0 ]] ; then
	error $0 $LINENO
	exit 1
fi
source xDevil_cp.in.sh
if [[ $? != 0 ]] ; then
	error $0 $LINENO
	exit 1
fi
source $xDevil_M_Dir/init/create.dir.sh
if [[ $? != 0 ]] ; then
	error $0 $LINENO
	exit 1
fi
set >configure.gen.set.txt