#!/bin/bash -ue
#
#source $Clip_F_F
#if [ $? != 0 ] ; then
#	echo "Configuration for $PWD"
#	error $PWD $0
#	exit 1
#fi
##source configure.ini
#if [ $? != 0 ] ; then
#	echo "Configuration for $PWD"
#	error $PWD $0
#	exit 1
#fi
#Clipcfg_h="$Clip_I_Dir/clipcfg.h"
echo "[buid] $Clipcfg_h" >&0
echo "[buid] $Clipcfg_h" >&1
#
# 		called from ../configure
#
#		NO NEEDS to export variables
#
OnScreen 1 'Configuration of clip tools & libraries'
OnScreen 0 'Some parameters need to be given to suit your needs'

##########################################################################################################
##																																			##
##				clipcfg.h																												##
##																																			##
##########################################################################################################


OnScreen 0 "configure: creating clipcfg.h .."
TempFile="$Clip_T_Dir/temp_clipcfg.$$.in"
rm -f$V $TempFile $TempFile.out

echo "" 																				>$Clipcfg_h
echo "/* Created automatically by 'configure'bin/clipcfg.h.sh */"	>>$Clipcfg_h
echo "/* file : $Clipcfg_h*/"													>>$Clipcfg_h
echo "#ifndef CLIP_CONFIG_H"													>>$Clipcfg_h
echo "#define CLIP_CONFIG_H"													>>$Clipcfg_h
echo																					>>$Clipcfg_h
export CLIP_VERSION=$(cat $Clip_C_Dir/release_version)
echo '#define CLIP_VERSION "'$CLIP_VERSION'"' 							>$TempFile
#echo '#define SEQ_NO "'$seq_no'"' 											>>$TempFile
echo '#define DLLIB "'$DLLIB'"' 												>>$TempFile
echo "#define ADDLIBS \"$ADDLIBS\"" 										>>$TempFile
echo '#define ADD_CFLAGS "'$ADD_CFLAGS'"' 								>>$TempFile
echo '#define DLLSUFF "'$DLLSUFF'"' 										>>$TempFile
echo '#define EXESUFF "'$Suff_Exe'"' 										>>$TempFile
echo '#define DLLREALSUFF "'$DLLREALSUFF'"' 								>>$TempFile
echo "#define CLIP_DLLEXPORT $CLIP_DLLEXPORT" 							>>$TempFile
echo "#define CLIP_DLLIMPORT $CLIP_DLLIMPORT" 							>>$TempFile
echo '#define NM_PRG "'$NM_PRG'"' 											>>$TempFile
export USE_AS=no
if [[ "$USE_AS" = no ]] ; then
	echo "#undef USE_AS" 														>>$TempFile
	echo "#define AS_PRG 0" 													>>$TempFile
else
	echo "#define USE_AS" 														>>$TempFile
	echo '#define AS_PRG "'$AS_PRG'"' 										>>$TempFile
fi

if [[ "$USE_LD" = yes ]] ; then
	echo "#define USE_LD" 														>>$TempFile
	echo '#define LD_PRG "'$LD_PRG'"' 										>>$TempFile
	echo '#define LD_END "'$LD_END'"' 										>>$TempFile
	echo '#define LDS_PRG "'$LDS_PRG'"' 									>>$TempFile
	echo '#define LDS_END "'$LDS_END'"' 									>>$TempFile
else
	echo "#undef USE_LD" 														>>$TempFile
	echo "#define LD_PRG 0" 													>>$TempFile
fi
echo '#define OSNAME "'$OSNAME'"' 											>>$TempFile
echo '#define ARCH "'$ARCH'"' 												>>$TempFile
echo "#define ARCH_$ARCH" 														>>$TempFile
echo "#define OS_$OSNAME" 														>>$TempFile
echo '#define SYSTEM "'$U_Name'"' 											>>$TempFile
echo "#define CLIP_ROOT \"$CLIPROOT\"" 									>>$TempFile
#echo "#define CLIPROOT \"$CLIPROOT\"" 										>>$TempFile
echo '#define BINDIR "'$BINDIR'"' 											>>$TempFile

if [[ "$CHECK_STACK" = yes ]] ; then
	echo "#define WITH_CHECK_STACK 1" 										>>$TempFile
else
	echo "#undef WITH_CHECK_STACK" 											>>$TempFile
fi

#if [ -n "$MEMDBGFLAGS" ] ; then
#	echo "#define MEMDBGFLAGS $MEMDBGFLAGS" 								>>$TempFile
#fi

if [ -n "$M_DBG" ] ; then
# echo "#define MEMDEBUG "$(memdebug/malloc_dbg -f memdebug/mallocrc $MEM_DEBUG 2>&- )>>$TempFile
 	dvar=$($CLIPROOT/clip/memdebug/malloc_dbg -f $CLIPROOT/clip/memdebug/mallocrc -b ${MEM_DEBUG} 2>&-)
# dvar=$(memdebug/malloc_dbg -f memdebug/mallocrc -b- ${MEM_DEBUG} 2>&-)
	echo "#define MEMDEBUG" $(eval $dvar; echo $MALLOC_DEBUG )		>>$TempFile
	echo "#define MALLOC_FUNC_CHECK" 										>>$TempFile
fi

if [[ -f /usr/lib/gcc/$RealArch*/*/include/float.h ]] ; then
	echo "#define HAVE_FLOAT_H" 												>>$TempFile
fi

if [ -f /usr/include/sys/io.h ] ; then
	echo "#define HAVE_SYS_IO_H" 												>>$TempFile
	echo "#define HAVE_MMAN_H"													>>$TempFile
	echo "#define HAVE_VFS_H"													>>$TempFile
	echo "#define HAVE_STATVFS_H"												>>$TempFile
else
	echo "#define NO_MMAP"														>>$TempFile
fi

if [ -f /usr/bin/wget ] ; then
	echo '#define WGET_PRG "/usr/bin/wget"' 								>>$TempFile
else
	if [ -f /usr/local/bin/wget ] ; then
		echo '#define WGET_PRG "/usr/local/bin/wget"'					>>$TempFile
	else
		Add_Bin "WGET" 'clip development'
	fi
fi
if [ -f /usr/include/zlib.h ] ; then
	echo '#define HAVE_ZLIB_H' 												>>$TempFile
else
	echo 'Warning: system do not have installed ZLIB development'
fi
if [ -f /usr/include/regex.h ] ; then
	echo '#define HAVE_REGEX_H' 												>>$TempFile
else
	echo 'Warning: system do not have installed REGEX development'
fi

if [ -f /usr/include/readline/readline.h ] ; then
	echo "#define HAVE_READLINE_H"											>>$TempFile
	echo "
#include <stdio.h>
#include <readline/readline.h>
int main(int argc, char **argv) { rl_already_prompted=1; return 0;}
" > /tmp/$$.c
	$CC -o /tmp/$$ /tmp/$$.c -L /lib $ReadLineLibs
	##	>/dev/null
	if [ $? = 0 ] ; then
		echo "#define HAVE_READLINE_ALREADY_PROMPTED"					>>$TempFile
	else
		echo "Warning: old READLINE version"
	fi
	rm -f$V /tmp/$$* >/dev/null
fi

if [ "$OSNAME" = "SUNOS" ] ; then
	echo "#undef USE_UTMP"														>>$TempFile
else

	echo "
#include <utmp.h>
int main(int argc, char **argv) { setutent(); return 0;}
 " > /tmp/$$.c
	$CC -o /tmp/$$ /tmp/$$.c	>/dev/null
	if [ $? = 0 ] ; then
		echo "#define USE_UTMP 1"												>>$TempFile
	else
		echo "#undef USE_UTMP"													>>$TempFile
	fi
	rm -f$V /tmp/$$* >/dev/null
fi

echo "
#include <sys/time.h>
int main(int argc, char **argv) { struct timeval tv; gettimeofday(&tv, 0); return 0;}
 " > /tmp/$$.c
$CC -o /tmp/$$ /tmp/$$.c	>/dev/null
if [ $? = 0 ] ; then
	echo "#define HAVE_GETTIMEOFDAY 1"										>>$TempFile
else
	echo "#undef HAVE_GETTIMEOFDAY"											>>$TempFile
fi
rm -f$V /tmp/$$* >/dev/null




# ptmx support
#
if [ -c /dev/ptmx -a -f /usr/include/stropts.h ] ; then
	echo "#define USE_PTS"														>>$TempFile
fi


if [ -n "$LibDir" ] ; then
	echo "#define LibDir $LibDir"												>>$TempFile
fi

if [ -n "$STD_LIBDIR" ] ; then
	echo "#define STD_LIBDIR $STD_LIBDIR"									>>$TempFile
fi

if [ -n "$StdLibDir" ] ; then
	echo "#define STD_LIB_DIR \"$StdLibDir\""								>>$TempFile
fi

if [ -n "$USE_WCHARS" ] ; then
	echo "#define USE_WCHARS"													>>$TempFile
fi

if [ -n "$NM_UNDERSCORE" ] ; then
	echo "#define NM_UNDERSCORE"												>>$TempFile
fi

if [ "$USE_TASKS" = yes ] ; then
	echo "#define USE_TASKS 1"													>>$TempFile
else
	echo "#undef USE_TASKS"														>>$TempFile
fi

if [ -n "$FORCEALIGN" ] ; then
	echo "#define FORCEALIGN $FORCEALIGN"									>>$TempFile
fi

if [ -f /usr/include/gpm.h ] ; then
	echo "#define HAVE_GPM_H"													>>$TempFile
fi

if [ "$Po_compat" = yes ] ; then
	echo "#define PO_COMPAT 1"													>>$TempFile
	echo "#define PO_COMPAT_CHAR 0177"										>>$TempFile
else
	echo "#undef PO_COMPAT"														>>$TempFile
fi

echo "#define ICONV_INC \"$ICONV_INC\"" 									>>$TempFile
if [ "$HAVE_ICONV" = yes ] ; then
	echo "#define HAVE_ICONV 1" 												>>$TempFile
else
	echo "#undef HAVE_ICONV"													>>$TempFile
fi
echo '#define DEFAULT_FONTPATH "/usr/share/fonts/truetype"'			>>$TempFile
echo '#define PATHSEPARATOR ":"'												>>$TempFile
case "$OSNAME" in
CYGWIN*)
	echo "#define _WIN32"														>>$TempFile
 ;;
MINGW*)
	echo
	echo "#define random rand"													>>$TempFile
	echo "#define srandom srand"												>>$TempFile
	echo "#define ftruncate chsize"											>>$TempFile
	echo
 #echo "#define _WIN32"															>>$TempFile
 ;;
esac


sort -u -d <$TempFile -o $TempFile.out

cat $TempFile.out																>>$Clipcfg_h
echo																				>>$Clipcfg_h

echo "#endif"																	>>$Clipcfg_h

#rm -f$V $TempFile $TempFile.out >/dev/null

OnScreen 1 ". $Clipcfg_h done."

############################################################################################################
##																																		 	##
##		end	clipcfg.h																											 	##
##																																		 	##
############################################################################################################
