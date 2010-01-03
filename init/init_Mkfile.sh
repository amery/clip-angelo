#!/bin/bash 
#
if [ -f $Clip_S_Dir/VerbosityLog.setup.ini ] ; then
	VerbosityLog=$(cat $Clip_S_Dir/VerbosityLog.setup.ini)
else
	VerbosityLog=-1
fi
echo "[build] $Clip_M_Dir/Makefile.ini" >&0
echo "[build] $Clip_M_Dir/Makefile.ini" >&1
OnScreen 1 "Creating $Clip_M_Dir/Makefile.ini"
echo "[build] $Clip_M_Dir/configure.ini" >&0
echo "[build] $Clip_M_Dir/configure.ini" >&1
OnScreen 1 "Creating $Clip_M_Dir/configure.ini"
echo "[build] $Clip_F_CFG" >&0
echo "[build] $Clip_F_CFG" >&1
OnScreen 1 "Creating $Clip_F_CFG"
echo "#" 																			>$Clip_M_Dir/Makefile.ini
echo "# Created automatically by configure bin/init_Mkfile.sh" 	>>$Clip_M_Dir/Makefile.ini
echo "#" 																			>>$Clip_M_Dir/Makefile.ini
echo "# file : Makefile.ini"													>>$Clip_M_Dir/Makefile.ini

echo "##"  																			>$Clip_M_Dir/configure.ini
echo "## automatically genarated by configure 'init_Mkfile.sh'"  >>$Clip_M_Dir/configure.ini
echo "##"  																			>>$Clip_M_Dir/configure.ini
echo "## file : configure.ini"  												>>$Clip_M_Dir/configure.ini

echo "#" 																			>$Clip_F_CFG
echo "# Created automatically by configure bin/init_Mkfile.sh" 	>>$Clip_F_CFG
echo "#" 																			>>$Clip_F_CFG
echo "# file : $Clip_F_CFG" 													>>$Clip_F_CFG

export makefile_ini="$Clip_T_Dir/temp_ini_M.$$.in"
rm -f$V $makefile_ini $makefile_ini.out
export configure_ini="$Clip_T_Dir/temp_ini_M2.$$.in"
rm -f$V $configure_ini $configure_ini.out
export clip_cfg_cfg="$Clip_T_Dir/temp_ini_M3.$$.in"
rm -f$V $clip_cfg_cfg $clip_cfg_cfg.out
#		set >$(date +%s).configure.$LINENO.set.txt

export >/dev/null

export List=""
[ -f $Clip_S_Dir/Makefile.setup.ini ] && List="$(cat $Clip_S_Dir/Makefile.setup.ini )"
while [ -z "$List" ] ; do
	$Clip_C_Dir/P_Mkfile.sh
	if [ $? != 0 ] ; then
		echo "Configuration for $PWD"
		error $Clip_C_Dir/P_Mkfile.sh
		exit 1
	fi
	export List="$(cat $Clip_S_Dir/Makefile.setup.ini )"
done
export Flagger=0
while [ $Flagger = 0 ] ; do
	export Flagger=1
	for nii in $List ; do
   	if [ -f $Clip_S_Dir/$nii.setup.ini ] ; then
			export xx="$(cat $Clip_S_Dir/$nii.setup.ini)"
			echo "export $nii=$xx" 								>>$makefile_ini
		else
			export Flagger=0
		fi
	done
	if [ $Flagger = 0 ] ; then
		$Clip_C_Dir/P_Mkfile.sh
		if [ $? != 0 ] ; then
			echo "Configuration for $PWD"
			error $Clip_C_Dir/P_Mkfile.sh
			exit 1
		fi
		export List="$(cat $Clip_S_Dir/Makefile.setup.ini )"
	fi
done
#
#ClipParameters="$(General_Parameter_For_clip_dir)"
#
export >/dev/null
export List=""
[ -f $Clip_S_Dir/clip.setup.ini ] && List="$(cat $Clip_S_Dir/clip.setup.ini )"
while [ -z "$List" ] ; do
	$Clip_C_Dir/P_clip.sh
	if [ $? != 0 ] ; then
		echo "Configuration for $PWD"
		error $Clip_C_Dir/P_clip.sh
		exit 1
	fi
	export List="$(cat $Clip_S_Dir/clip.setup.ini )"
done
export Flagger=0
while [ $Flagger = 0 ] ; do
	export Flagger=1
	for nii in $List ; do
   	if [ -f $Clip_S_Dir/$nii.setup.ini ] ; then
			export xx="$(cat $Clip_S_Dir/$nii.setup.ini)"
			echo "export $nii=\"$xx\"" 							>>$makefile_ini
			echo "export $nii=\"$xx\"" 							>>$configure_ini
		else
			export Flagger=0
		fi
	done
	if [ $Flagger = 0 ] ; then
		$Clip_C_Dir/P_clip.sh
		if [ $? != 0 ] ; then
			echo "Configuration for $PWD"
			error $Clip_C_Dir/P_clip.sh
			exit 1
		fi
		export List="$(cat $Clip_S_Dir/clip.setup.ini )"
	fi
done
#
#		CliplibsParameters=$(General_Parameter_For_cliplibs_dir)
#
export >/dev/null
export List=""
[ -f $Clip_S_Dir/cliplibs.setup.ini ] && List="$(cat $Clip_S_Dir/cliplibs.setup.ini )"
while [ -z "$List" ] ; do
	$Clip_C_Dir/P_cliplibs.sh
	if [ $? != 0 ] ; then
		echo "Configuration for $PWD"
		error $Clip_C_Dir/P_cliplibs.sh
		exit 1
	fi
	List="$(cat $Clip_S_Dir/cliplibs.setup.ini )"
done
export Flagger=0
while [ $Flagger = 0 ] ; do
	export Flagger=1
	for nii in $List ; do
   	if [ -f $Clip_S_Dir/$nii.setup.ini ] ; then
			export xx="$(cat $Clip_S_Dir/$nii.setup.ini)"
			echo "export $nii=\"$xx\"" 							>>$makefile_ini
			echo "export $nii=\"$xx\"" 							>>$configure_ini
		else
			export Flagger=0
		fi
	done
	if [ $Flagger = 0 ] ; then
		$Clip_C_Dir/P_cliplibs.sh
		if [ $? != 0 ] ; then
			echo "Configuration for $PWD"
			error $Clip_C_Dir/P_cliplibs.sh
			exit 1
		fi
		export List="$(cat $Clip_S_Dir/cliplibs.setup.ini )"
	fi
done

#
#		PrgParameters=$(General_Parameter_For_prg_dir)
#
export List=""
export >/dev/null
[ -f $Clip_S_Dir/prg.setup.ini ] && List="$(cat $Clip_S_Dir/prg.setup.ini )"
while [ -z "$List" ] ; do
	$Clip_C_Dir/P_prg.sh
	if [ $? != 0 ] ; then
		echo "Configuration for $PWD"
		error $Clip_C_Dir/P_prg.sh
		exit 1
	fi
	export List="$(cat $Clip_S_Dir/prg.setup.ini )"
done
export Flagger=0
while [ $Flagger = 0 ] ; do
	export Flagger=1
	for nii in $List ; do
   	if [ -f $Clip_S_Dir/$nii.setup.ini ] ; then
			export xx="$(cat $Clip_S_Dir/$nii.setup.ini)"
			echo "export $nii=\"$xx\"" 							>>$makefile_ini
			echo "export $nii=\"$xx\"" 							>>$configure_ini
		else
			export Flagger=0
		fi
	done
	if [ $Flagger = 0 ] ; then
		$Clip_C_Dir/P_prg.sh
		if [ $? != 0 ] ; then
			echo "Configuration for $PWD"
			error $Clip_C_Dir/P_prg.sh
			exit 1
		fi
		export List="$(cat $Clip_S_Dir/prg.setup.ini )"
	fi
done
#echo Debug_Make : $Debug_Make
if [[ "$Debug_Make" = \"1\" ]] ; then
	let Debug_Make=1
else
	let Debug_Make=0
fi
#xx=""
echo Debug_Make : $Debug_Make
#read -n 1 -p "Debug" xx
echo "ARCH=$ARCH" >&0
echo "C64=$C64" 	>&0
#Clip_cp="$Clip_B_Dir/clip_cp$Suff_Script"
#Clip_msgmerge="$Clip_B_Dir/clip_msgmerge$Suff_Script"
#Clip_msgfmt="$Clip_B_Dir/clip_msgfmt$Suff_Script"
#Clip_makelib="$Clip_B_Dir/clip_makelib$Suff_Script"
#Clip_makeslib="$Clip_B_Dir/clip_makeslib$Suff_Script"
echo "#!/bin/bash " >$Clip_cp
echo "#!/bin/bash " >$Clip_msgmerge
echo "#!/bin/bash " >$Clip_msgfmt
chmod +x $Clip_msgmerge
chmod +x $Clip_msgfmt
chmod +x $Clip_cp
echo "export ADD_CFLAGS=$ADD_CFLAGS" 							>>$makefile_ini
echo "export ADD_CFLAGS=\"$ADD_CFLAGS\"" 					>>$configure_ini
echo "export ADD_CFLAGS=\"$ADD_CFLAGS\"" 								>>$clip_cfg_cfg
echo "export ADD_CFLAGS=\"$ADD_CFLAGS\"" 								>>$Clip_cp
echo "export ADDOBJS=" 												>>$makefile_ini
echo "export ADDOBJS2=" 											>>$makefile_ini
echo "export ADDLIBS=$ADDLIBS" 									>>$makefile_ini
echo "export ADDLIBS=\"$ADDLIBS\"" 							>>$configure_ini
echo "export ADDLIBS=\"$ADDLIBS\"" 										>>$clip_cfg_cfg
echo "export ADDLIBS=\"$ADDLIBS\"" 										>>$Clip_cp
echo "export ADDLIBS2=" 											>>$makefile_ini
echo "export ARCH=$ARCH" 	 										>>$makefile_ini
echo "export ARCH=$ARCH" 										>>$configure_ini
echo "export AS_PRG=$AS_PRG" 	 									>>$makefile_ini
#echo "export ATARGET = \$(TARGET)\$(DLLLIBSUFF)" 			>>$makefile_ini
echo "export BINDIR=$BINDIR" 										>>$makefile_ini
echo "export BINDIR=\"$BINDIR\"" 							>>$configure_ini
echo "export C64=$C64" 												>>$makefile_ini
echo "export C64=$C64" 											>>$configure_ini
echo "export CC=$CC" 												>>$makefile_ini
echo "export CC=$CC" 											>>$configure_ini
echo "export CC=$CC" 														>>$clip_cfg_cfg
echo "export CC=$CC" 														>>$Clip_cp
echo "export C_FLAGS=$C_FLAGS" 										>>$makefile_ini
#echo "export Clipar=$Clipar"										>>$makefile_ini
#echo "export Clipar=\"$Clipar\"" 							>>$configure_ini
echo "export Clip_B_Dir=$Clip_B_Dir" 							>>$makefile_ini
echo "export Clip_B_Dir=\"$Clip_B_Dir\"" 					>>$configure_ini
echo "export Clip_C_Dir=$Clip_C_Dir" 							>>$makefile_ini
echo "export Clip_C_Dir=\"$Clip_C_Dir\"" 					>>$configure_ini
echo "export Clipcfg_h=$Clipcfg_h" 								>>$makefile_ini
echo "export Clipcfg_h=\"$Clipcfg_h\"" 					>>$configure_ini
#echo "export Clip_cld=$Clip_cld"									>>$makefile_ini
#echo "export Clip_conv=$Clip_conv"								>>$makefile_ini
#echo "export Clip_conv=\"$Clip_conv\"" 					>>$configure_ini
#echo "export Clip_cp=$Clip_cp" 									>>$makefile_ini
#echo "export Clip_cp=\"$Clip_cp\"" 							>>$configure_ini
#echo "export Clip_dbg=$Clip_dbg" 								>>$makefile_ini
#echo "export Clip_dbg=\"$Clip_dbg\"" 						>>$configure_ini
echo "export Clip_D_Dir=$Clip_D_Dir" 							>>$makefile_ini
echo "export Clip_D_Dir=\"$Clip_D_Dir\"" 					>>$configure_ini
#echo "export Clip_debugger=$Clip_debugger" 					>>$makefile_ini
#echo "export Clip_debugger=\"$Clip_debugger\"" 			>>$configure_ini
echo "export CLIP_DLLEXPORT=$CLIP_DLLEXPORT" 				>>$makefile_ini
echo "export CLIP_DLLIMPORT=$CLIP_DLLIMPORT" 				>>$makefile_ini
echo "export Clip_F_CFG=$Clip_F_CFG" 							>>$makefile_ini
echo "export Clip_F_CFG=\"$Clip_F_CFG\"" 					>>$configure_ini
echo "export Clip_F_F=$Clip_F_F" 								>>$makefile_ini
echo "export Clip_F_F=\"$Clip_F_F\"" 						>>$configure_ini
#echo "export Clip_hashextract=$Clip_hashextract" 			>>$makefile_ini
#echo "export Clip_hashextract=\"$Clip_hashextract\"" 	>>$configure_ini
#echo "export Cliphash=$Cliphash" 								>>$makefile_ini
echo "export Cliphash=\"$Cliphash\"" 						>>$configure_ini
echo "export Clip=$Clip" 											>>$makefile_ini
#echo "export Clip=\"$Clip\"" 									>>$configure_ini
echo "export Clip_H_Dir=$Clip_H_Dir" 							>>$makefile_ini
echo "export Clip_H_Dir=\"$Clip_H_Dir\"" 					>>$configure_ini
echo "export Clip_I_Dir=$Clip_I_Dir" 							>>$makefile_ini
echo "export Clip_I_Dir=\"$Clip_I_Dir\"" 					>>$configure_ini
echo "export Clip_L_Dir=$Clip_L_Dir" 							>>$makefile_ini
echo "export Clip_L_Dir=\"$Clip_L_Dir\"" 					>>$configure_ini
echo "export CLIP_LIB=$CLIP_LIB" 								>>$makefile_ini
echo "export CLIP_LIB=\"$CLIP_LIB\"" 						>>$configure_ini
echo "export ClipLibrary=$ClipLibrary" 						>>$makefile_ini
echo "export ClipLibrary=\"$ClipLibrary\"" 				>>$configure_ini
echo "export CliplibsVerbose=$CliplibsVerbose" 				>>$makefile_ini
echo "export Clip_makelib=$Clip_makelib" 						>>$makefile_ini
#echo "export Clip_makelib=\"$Clip_makelib\"" 			>>$configure_ini
echo "export Clip_makeslib=$Clip_makeslib" 					>>$makefile_ini
#echo "export Clip_makeslib=\"$Clip_makeslib\"" 			>>$configure_ini
echo "export Clip_M_Dir=$Clip_M_Dir" 							>>$makefile_ini
echo "export Clip_M_Dir=\"$Clip_M_Dir\"" 					>>$configure_ini
echo "export Clip_msgfmt=$Clip_msgfmt" 						>>$makefile_ini
#echo "export Clip_msgfmt=\"$Clip_msgfmt\"" 				>>$configure_ini
echo "export Clip_msgmerge=$Clip_msgmerge"					>>$makefile_ini
#echo "export Clip_msgmerge=\"$Clip_msgmerge\"" 			>>$configure_ini
echo "export CLIP_NAMES=$CLIP_NAMES" 							>>$makefile_ini
echo "export CLIP_ROOT=$CLIPROOT" 								>>$makefile_ini
echo "export CLIP_ROOT=\"$CLIPROOT\"" 						>>$configure_ini
echo "export CLIP_ROOT=\"$CLIPROOT\"" 									>>$clip_cfg_cfg
echo "export CLIP_ROOT=\"$CLIPROOT\"" 									>>$Clip_cp
echo "export CLIPROOT=$CLIPROOT" 								>>$makefile_ini
echo "export CLIPROOT=\"$CLIPROOT\"" 						>>$configure_ini
echo "export CLIPROOT=\"$CLIPROOT\"" 									>>$Clip_cp
echo "export CLIPROOT=\"$CLIPROOT\"" 									>>$Clip_msgfmt
echo "export CLIPROOT=\"$CLIPROOT\"" 									>>$Clip_msgmerge
echo "export Clip_S_Dir=$Clip_S_Dir" 							>>$makefile_ini
echo "export Clip_S_Dir=\"$Clip_S_Dir\"" 					>>$configure_ini
echo "export CLIP_TASKS=$CLIP_TASKS" 							>>$makefile_ini
echo "export Clip_T_Dir=$Clip_T_Dir" 							>>$makefile_ini
echo "export Clip_T_Dir=\"$Clip_T_Dir\"" 					>>$configure_ini
#echo "export Clip_trans=$Clip_trans"							>>$makefile_ini
#echo "export Clip_trans=\"$Clip_trans\"" 					>>$configure_ini
echo "export ClipVerbose=$ClipVerbose" 						>>$makefile_ini
echo "export CONFIGURE=$CONFIGURE" 								>>$makefile_ini
echo "export CONFIGURE=\"$CONFIGURE\"" 					>>$configure_ini
echo "export CONFIGURE_DIR=$CONFIGURE_DIR" 					>>$makefile_ini
echo "export CONFIGURE_DIR=\"$CONFIGURE_DIR\"" 			>>$configure_ini
echo "export CONFIGURE_SH=$CONFIGURE_SH" 						>>$makefile_ini
echo "export CONFIGURE_SH=\"$CONFIGURE_SH\"" 			>>$configure_ini
echo "export DBG_LIB=$DBG_LIB" 									>>$makefile_ini
echo "export DBG_LIB=$DBG_LIB" 								>>$configure_ini
echo "export DBG_LIB=$DBG_LIB" 											>>$clip_cfg_cfg
echo "export DEBUGFLAGS=$DEBUGFLAGS" 							>>$makefile_ini
echo "export Debug_Make=$Debug_Make" 							>>$makefile_ini
echo "export Debug_Make=$Debug_Make" 						>>$configure_ini
echo "export Depend_h=" 											>>$makefile_ini
echo "export Depend_h=" 										>>$configure_ini
echo "export DLLIB=$DLLIB" 										>>$makefile_ini
echo "export DLLLIBSUFF=$DLLLIBSUFF" 							>>$makefile_ini
echo "export DLLLIBSUFF=$DLLLIBSUFF" 						>>$configure_ini
echo "export DLLREALSUFF=$DLLREALSUFF" 						>>$makefile_ini
echo "export DLLREALSUFF=$DLLREALSUFF" 					>>$configure_ini
echo "export DLLREALSUFF=$DLLREALSUFF" 								>>$clip_cfg_cfg
echo "export DLLREALSUFF=$DLLREALSUFF" 								>>$Clip_cp
echo "export DLLSUFF=$DLLSUFF" 									>>$makefile_ini
echo "export DLLSUFF=$DLLSUFF" 								>>$configure_ini
echo "export DLLSUFF=$DLLSUFF" 											>>$clip_cfg_cfg
echo "export DLLSUFF=$DLLSUFF" 											>>$Clip_cp
echo "export EXESUFF=$Suff_Exe" 									>>$makefile_ini
echo "export EXESUFF=$Suff_Exe" 								>>$configure_ini
echo "export EXESUFF=$Suff_Exe" 										>>$clip_cfg_cfg
echo "export EXESUFF=$Suff_Exe" 										>>$Clip_cp
echo "export FORCEALIGN=$FORCEALIGN" 							>>$makefile_ini
#echo "export Genlist=$Genlist" 									>>$makefile_ini
#echo "export Genlist=\"$Genlist\"" 							>>$configure_ini
#echo "export GenMakefile=$GenMakefile" 						>>$makefile_ini
#echo "export GenMakefile=\"$GenMakefile\"" 				>>$configure_ini
#echo "export Gen_tbl=$Gen_tbl" 									>>$makefile_ini
#echo "export Gen_tbl=\"$Gen_tbl\"" 							>>$configure_ini
echo "export Hash_Dir=$Hash_Dir" 								>>$makefile_ini
echo "export Hash_Dir=\"$Hash_Dir\"" 						>>$configure_ini
echo "export HAVE_ICONV=$HAVE_ICONV" 							>>$makefile_ini
echo "export ICONV_LIB=$ICONV_LIB" 								>>$makefile_ini
echo "export ICONV_LIB=\"$ICONV_LIB\"" 					>>$configure_ini
echo "export ICONV_LIB_DIR=$ICONV_LIB_DIR" 					>>$makefile_ini
echo "export ICONV_LIB_DIR=\"$ICONV_LIB_DIR\"" 			>>$configure_ini
echo "export LD_END=$LD_END" 										>>$makefile_ini
echo "export LD_PRG=$LD_PRG" 										>>$makefile_ini
echo "export LDS_END=$LDS_END" 									>>$makefile_ini
echo "export LDS_PRG=$LDS_PRG" 									>>$makefile_ini
echo "export LIBC=$LIBC" 											>>$makefile_ini
echo "export LibDir=$LibDir" 										>>$makefile_ini
echo "export LibDir=$LibDir" 									>>$configure_ini
echo "export LibDir=$LibDir" 												>>$clip_cfg_cfg
echo "export LIBS=" 													>>$makefile_ini
echo "export LIBS2=" 												>>$makefile_ini
echo "export LogFileLog=$LogFileLog" 							>>$makefile_ini
echo "export LogFileLog=$LogFileLog" 						>>$configure_ini
echo "export MAKE=$MAKE" 											>>$makefile_ini
echo "export Makefile_01_in=$Makefile_01_in" 				>>$makefile_ini
echo "export Makefile_01_in=\"$Makefile_01_in\"" 		>>$configure_ini
echo "export Makefile_gen_in=$Makefile_gen_in" 				>>$makefile_ini
echo "export Makefile_gen_in=\"$Makefile_gen_in\"" 	>>$configure_ini
echo "export M_DBG=$M_DBG" 										>>$makefile_ini
echo "export MDBG=$M_DBG" 											>>$makefile_ini
echo "export MDBG=$M_DBG" 										>>$configure_ini
echo "export MDBG=$M_DBG" 													>>$clip_cfg_cfg
echo "export M_LIB=$M_LIB" 										>>$makefile_ini
echo "export M_LIB=$M_LIB" 									>>$configure_ini
echo "export M_LIB=$M_LIB" 												>>$clip_cfg_cfg
echo "export M_LIB=$M_LIB" 												>>$Clip_cp
echo "export NM_PRG=$NM_PRG" 										>>$makefile_ini
echo "export NM_PRG=$NM_PRG" 											>>$clip_cfg_cfg
echo "export NM_PRG=$NM_PRG" 											>>$Clip_cp
echo "export NM_UNDERSCORE=$NM_UNDERSCORE" 					>>$makefile_ini
echo "export NM_UNDERSCORE=$NM_UNDERSCORE" 							>>$clip_cfg_cfg
echo "export NM_UNDERSCORE=$NM_UNDERSCORE" 							>>$Clip_cp
echo "export NO_GETTEXT=$NO_GETTEXT" 							>>$makefile_ini
echo "export NO_GETTEXT=$NO_GETTEXT" 									>>$Clip_msgfmt
echo "export NO_GETTEXT=$NO_GETTEXT" 									>>$Clip_msgmerge
echo "export OBJS=" 													>>$makefile_ini
echo "export OBJS2=" 												>>$makefile_ini
echo "export OS_$OSNAME=yes" 										>>$makefile_ini
echo "export OSNAME=$OSNAME" 										>>$makefile_ini
echo "export OSNAME=$OSNAME" 									>>$configure_ini
echo "export OSNAME=$OSNAME" 												>>$clip_cfg_cfg
echo "export OSNAME=$OSNAME" 												>>$Clip_cp
echo "export OS_NAME=$OS_NAME" 									>>$makefile_ini
echo "export OS_NAME=$OS_NAME" 								>>$configure_ini
echo "export OS_NAME=$OS_NAME" 											>>$Clip_cp
if [ -n $OS_LINUX ] ; then
	echo $OS_LINUX
	echo "export OS_LINUX=yes" 									>>$makefile_ini
	echo "export OS_LINUX=yes" 								>>$configure_ini
	echo "export OS_LINUX=yes" 											>>$clip_cfg_cfg
	echo "export OS_LINUX=yes" 											>>$Clip_cp
fi
echo "export PKGCONFIG=$PKGCONFIG" 								>>$makefile_ini
echo "export PKGCONFIG=$PKGCONFIG" 							>>$configure_ini
echo "export PO_BINS=$PO_BINS" 									>>$makefile_ini
#echo "export Po_compat=$Po_compat" 								>>$makefile_ini
#echo "export Po_compat=\"$Po_compat\"" 					>>$configure_ini
#echo "export PO_COMPAT=$Po_compat" 								>>$makefile_ini
#echo "export Po_extr=$Po_extr" 									>>$makefile_ini
#echo "export Po_extr=\"$Po_extr\"" 							>>$configure_ini
echo "export PO_FROM_COMPAT=$PO_FROM_COMPAT" 				>>$makefile_ini
echo "export PO_FROM_COMPAT=$PO_FROM_COMPAT" 						>>$Clip_msgfmt
echo "export PO_FROM_COMPAT=$PO_FROM_COMPAT" 						>>$Clip_msgmerge
#echo "export Po_subst=$Po_subst" 								>>$makefile_ini
#echo "export Po_subst=\"$Po_subst\"" 						>>$configure_ini
echo "export PO_TO_COMPAT=$PO_TO_COMPAT" 						>>$makefile_ini
echo "export PO_TO_COMPAT=$PO_TO_COMPAT" 								>>$Clip_msgfmt
echo "export PO_TO_COMPAT=$PO_TO_COMPAT" 								>>$Clip_msgmerge
echo "export PROG=" 													>>$makefile_ini
echo "export PROG=" 												>>$configure_ini
echo "export PrgVerbose=$PrgVerbose" 							>>$makefile_ini
echo "export READLINE_LIBS=$ReadLineLibs" 					>>$makefile_ini
echo "export ReadLineLibs=$ReadLineLibs" 						>>$makefile_ini
echo "export RealArch=$RealArch" 	 							>>$makefile_ini
echo "export release_date=$release_date" 						>>$makefile_ini
echo "export release_date=\"$release_date\"" 			>>$configure_ini
echo "export ReleaseDir=$ReleaseDir" 							>>$makefile_ini
echo "export release_version=$release_version" 				>>$makefile_ini
echo "export release_version=\"$release_version\""		>>$configure_ini
#echo "export RTARGET = \$(TARGET)\$(DLLREALSUFF)" 		>>$makefile_ini
echo "export seq_no=$seq_no" 										>>$makefile_ini
echo "export seq_no=\"$seq_no\""								>>$configure_ini
echo "export sequence_date=$sequence_date" 					>>$makefile_ini
echo "export sequence_date=\"$sequence_date\""			>>$configure_ini
echo "export sequence_release=$sequence_release" 			>>$makefile_ini
echo "export sequence_release=\"$sequence_release\""	>>$configure_ini
#echo "StandardDepend=$StandardDepend" 				>>$makefile_ini
#echo "StandardDepend=\"$StandardDepend\"" 		>>$configure_ini
#echo "STARGET = \$(TARGET)\$(DLLSUFF)" 				>>$makefile_ini
echo "export STATICLINK=$STATICLINK" 							>>$makefile_ini
echo "export StdIncDir=$StdIncDir" 								>>$makefile_ini
echo "export StdIncDir=\"$StdIncDir\"" 					>>$configure_ini
echo "export STD_LIBDIR=$STD_LIBDIR" 							>>$makefile_ini
echo "export STD_LIBDIR=$STD_LIBDIR" 						>>$configure_ini
echo "export STD_LIBDIR=$STD_LIBDIR" 									>>$clip_cfg_cfg
echo "export STD_LIBDIR=$STD_LIBDIR" 									>>$Clip_cp
echo "export StdLibDir=$StdLibDir" 								>>$makefile_ini
echo "export StdLibDir=\"$StdLibDir\"" 					>>$configure_ini
echo "export Suff_Exe=$Suff_Exe" 								>>$makefile_ini
echo "export Suff_Exe=$Suff_Exe" 							>>$configure_ini
echo "export Suff_Script=$Suff_Script" 						>>$makefile_ini
echo "export Suff_Script=$Suff_Script" 					>>$configure_ini
#echo "export TARGET=lib\$(PROG)" 								>>$makefile_ini
echo "export TERMCAP=$TERMCAP" 									>>$makefile_ini
echo "export TERMCAP=$TERMCAP" 								>>$configure_ini
echo "export TERMCAP=$TERMCAP" 											>>$clip_cfg_cfg
echo "export U_Name=$U_Name" 										>>$makefile_ini
echo "export USE_AS=$USE_AS" 										>>$makefile_ini
echo "export USE_GTK=$USE_GTK" 									>>$makefile_ini
echo "export USE_LD=$USE_LD" 										>>$makefile_ini
echo "export USE_MEMDBG=$USE_MEMDBG" 							>>$makefile_ini
echo "export USE_TASKS=$USE_TASKS" 								>>$makefile_ini
echo "export USE_WCHARS=$USE_WCHARS" 							>>$makefile_ini
echo "export U_Ver=$U_Ver" 										>>$makefile_ini
echo "export V=$V" 													>>$makefile_ini
echo "export V=$V" 												>>$configure_ini
echo "export V=$V" 															>>$clip_cfg_cfg
echo "export VerbosityLog=$VerbosityLog" 						>>$makefile_ini
echo "export VerbosityLog=$VerbosityLog" 					>>$configure_ini
echo "export WaitForCheck=$WaitForCheck" 					>>$configure_ini
echo "export WAIT_SECONDS=$WAIT_SECONDS" 						>>$makefile_ini
echo "export XCLIP=$XCLIP" 										>>$makefile_ini
echo "export YACC=$YACC" 											>>$makefile_ini
echo "echo $CLIP_NAMES " 													>>$Clip_cp
sort -u <$makefile_ini -o $makefile_ini.out

cat $makefile_ini.out 										>>$Clip_M_Dir/Makefile.ini

rm -f$V $makefile_ini $makefile_ini.out >/dev/null

sleep 1

#rm -f$V $makefile_ini $makefile_ini.out
##
## Debug_Make define functions
##
##			Title
##
let VerbosityLog=$VerbosityLog
echo VerbosityLog : $VerbosityLog
echo "define TITLE"																													>>$Clip_M_Dir/Makefile.ini
echo " 	@echo \$(PWD)																										>&0"	>>$Clip_M_Dir/Makefile.ini
echo " 	@echo Executing command : \$(MAKE) \$(MAKEFLAGS) \$(MAKECMDGOALS) \$(ADDMAKECMDGOALS) 		>&0"	>>$Clip_M_Dir/Makefile.ini
if [[ $VerbosityLog -le 2 ]] ; then
	echo " 	@echo \$(PWD)																									>&1"	>>$Clip_M_Dir/Makefile.ini
	echo " 	@echo Executing command : \$(MAKE) \$(MAKEFLAGS) \$(MAKECMDGOALS) \$(ADDMAKECMDGOALS)	>&1"	>>$Clip_M_Dir/Makefile.ini
fi
if [[ $VerbosityLog -eq 2 ]] ; then
	echo " 	@echo \$(PWD)																									>&2"	>>$Clip_M_Dir/Makefile.ini
	echo " 	@echo Executing command : \$(MAKE) \$(MAKEFLAGS) \$(MAKECMDGOALS) \$(ADDMAKECMDGOALS)	>&2"	>>$Clip_M_Dir/Makefile.ini
fi
echo " 	@echo 																												>&0"	>>$Clip_M_Dir/Makefile.ini
echo " 	@sleep .1"																													>>$Clip_M_Dir/Makefile.ini
echo "endef"																															>>$Clip_M_Dir/Makefile.ini
##
##			Trace_o
##
echo "define Trace_o"																												>>$Clip_M_Dir/Makefile.ini
if [[ $Debug_Make -eq 1 ]] ; then
	echo "	@echo \"[Making]\" : \$(PWD)/\$@ because of : \$(sort \$? )										>&0"	>>$Clip_M_Dir/Makefile.ini
else
	echo "	@echo \"[Making]\" : \$(PWD)/\$@																			>&0"	>>$Clip_M_Dir/Makefile.ini
fi
if [[ $VerbosityLog -le 2 ]] ; then
	echo "VerbosityLog -le 2"
	if [[ $Debug_Make -eq 1 ]] ; then
		echo "	@echo \"[Making]\" : \$(PWD)/\$@ because of : \$(sort \$? )									>&1"	>>$Clip_M_Dir/Makefile.ini
	else
		echo "	@echo \"[Making]\" : \$(PWD)/\$@ 																	>&1"	>>$Clip_M_Dir/Makefile.ini
	fi
fi
if [[ $VerbosityLog -eq 2 ]] ; then
	echo "VerbosityLog -eq 2"
	if [[ $Debug_Make -eq 1 ]] ; then
		echo "	@echo \"[Making]\" : \$(PWD)/\$@ because of : \$(sort \$? )									>&2"	>>$Clip_M_Dir/Makefile.ini
	else
		echo "	@echo \"[Making]\" : \$(PWD)/\$@																		>&2"	>>$Clip_M_Dir/Makefile.ini
	fi
fi
echo " 	@sleep .1"																													>>$Clip_M_Dir/Makefile.ini
echo "endef"																															>>$Clip_M_Dir/Makefile.ini
echo Debug_Make : $Debug_Make
##
##			Trace_c
##
echo "define Trace_c"																												>>$Clip_M_Dir/Makefile.ini
echo "	\$(Trace_o)"																												>>$Clip_M_Dir/Makefile.ini
echo "	@echo \"[CC]\" \$@ 																								>&0"	>>$Clip_M_Dir/Makefile.ini
if [[ $VerbosityLog -le 2 ]] ; then
	echo "	@echo \"[CC]\" \$@ 																							>&1"	>>$Clip_M_Dir/Makefile.ini
fi
if [[ $VerbosityLog -eq 2 ]] ; then
	echo "	@echo \"[CC]\" \$@ 																							>&2"	>>$Clip_M_Dir/Makefile.ini
fi
echo " 	@sleep .1"																													>>$Clip_M_Dir/Makefile.ini
echo "endef"																															>>$Clip_M_Dir/Makefile.ini
##
##			Trace_prg
##
echo "define Trace_prg"																												>>$Clip_M_Dir/Makefile.ini
echo "	\$(Trace_o)"																												>>$Clip_M_Dir/Makefile.ini
echo "	@echo \"[clip]\" \$@ 																							>&0"	>>$Clip_M_Dir/Makefile.ini
if [[ $VerbosityLog -le 2 ]] ; then
	echo "	@echo \"[clip]\" \$@ 																						>&1"	>>$Clip_M_Dir/Makefile.ini
fi
if [[ $VerbosityLog -eq 2 ]] ; then
	echo "	@echo \"[clip]\" \$@ 																						>&2"	>>$Clip_M_Dir/Makefile.ini
fi
echo " 	@sleep .1"																													>>$Clip_M_Dir/Makefile.ini
echo "endef"																															>>$Clip_M_Dir/Makefile.ini
##
##			Trace_yacc
##
echo "define Trace_yacc"																											>>$Clip_M_Dir/Makefile.ini
echo "	\$(Trace_o)"																												>>$Clip_M_Dir/Makefile.ini
echo "	@echo \"[YACC]\" \$@ 																							>&0"	>>$Clip_M_Dir/Makefile.ini
if [[ $VerbosityLog -le 2 ]] ; then
	echo "	@echo \"[YACC]\" \$@ 																						>&1"	>>$Clip_M_Dir/Makefile.ini
fi
if [[ $VerbosityLog -eq 2 ]] ; then
	echo "	@echo \"[YACC]\" \$@ 																						>&2"	>>$Clip_M_Dir/Makefile.ini
fi
echo " 	@sleep .1"																													>>$Clip_M_Dir/Makefile.ini
echo "endef"																															>>$Clip_M_Dir/Makefile.ini
##
##			Trace_flex
##
echo "define Trace_flex"																											>>$Clip_M_Dir/Makefile.ini
echo "	\$(Trace_o)"																												>>$Clip_M_Dir/Makefile.ini
echo "	@echo \"[FLEX]\" \$@ 																							>&0"	>>$Clip_M_Dir/Makefile.ini
if [[ $VerbosityLog -le 2 ]] ; then
	echo "	@echo \"[FLEX]\" \$@ 																						>&1"	>>$Clip_M_Dir/Makefile.ini
fi
if [[ $VerbosityLog -eq 2 ]] ; then
	echo "	@echo \"[FLEX]\" \$@ 																						>&2"	>>$Clip_M_Dir/Makefile.ini
fi
echo " 	@sleep .1"																													>>$Clip_M_Dir/Makefile.ini
echo "endef"																															>>$Clip_M_Dir/Makefile.ini
##
##			Trace_lib
##
echo "define Trace_lib"																												>>$Clip_M_Dir/Makefile.ini
echo "	\$(Trace_o)"																												>>$Clip_M_Dir/Makefile.ini
echo "	@echo \"[build]\" \$@ 																							>&0"	>>$Clip_M_Dir/Makefile.ini
if [[ $VerbosityLog -le 2 ]] ; then
	echo "	@echo \"[build]\" \$@ 																						>&1"	>>$Clip_M_Dir/Makefile.ini
fi
if [[ $VerbosityLog -eq 2 ]] ; then
	echo "	@echo \"[build]\" \$@ 																						>&2"	>>$Clip_M_Dir/Makefile.ini
fi
echo " 	@sleep .1"																													>>$Clip_M_Dir/Makefile.ini
echo "endef"																															>>$Clip_M_Dir/Makefile.ini
##
##			Trace_lib2
##
echo "define Trace_lib2"																											>>$Clip_M_Dir/Makefile.ini
echo "	\$(Trace_o)"																												>>$Clip_M_Dir/Makefile.ini
echo "	@echo \"[build2]\" \$@ 																							>&0"	>>$Clip_M_Dir/Makefile.ini
if [[ $VerbosityLog -le 2 ]] ; then
	echo "	@echo \"[build2]\" \$@ 																						>&1"	>>$Clip_M_Dir/Makefile.ini
fi
if [[ $VerbosityLog -eq 2 ]] ; then
	echo "	@echo \"[build2]\" \$@ 																						>&2"	>>$Clip_M_Dir/Makefile.ini
fi
echo " 	@sleep .1"																													>>$Clip_M_Dir/Makefile.ini
echo "endef"																															>>$Clip_M_Dir/Makefile.ini

OnScreen 3 ". $Clip_M_Dir/Makefile.ini done."

sort -u <$configure_ini -o $configure_ini.out

cat -u $configure_ini.out 									>>$Clip_M_Dir/configure.ini

OnScreen 3 ". $Clip_M_Dir/configure.ini done."

sort -u <$clip_cfg_cfg -o $clip_cfg_cfg.out

cat -u $clip_cfg_cfg.out 									>>$Clip_F_CFG
#cp --remove-destination -fu$V $Clip_F_CFG $Clip_B_Dir/
cp --remove-destination -fu$V $Clip_cp $Clip_makelib
cp --remove-destination -fu$V $Clip_cp $Clip_makeslib
echo "USE_TASKS=$USE_TASKS">>$Clip_makeslib
cat bin/clip_cp.in >>$Clip_cp
cat bin/clip_makelib.in >>$Clip_makelib
cat bin/clip_makeslib.in >>$Clip_makeslib
cat bin/clip_msgfmt.in >>$Clip_msgfmt
cat bin/clip_msgmerge.in >>$Clip_msgmerge
OnScreen 3 ". $Clip_F_CFG done."

OnScreen 5 ". all configure files done $0"
#		set >$(date +%s).configure.$LINENO.set.txt
