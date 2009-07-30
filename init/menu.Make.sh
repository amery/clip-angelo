#!/bin/sh -u
#
#
if ! [ -x /bin/bash ] ; then
	echo "Please DO install bash packages"
	echo "Because the script files are written for BASH"
	echo "and not all shell's support the functions used in the scripts"
	read -n 1 -p "Press <enter> to continue"
fi
echo $*
export ReleaseDir=""
# general menu file.
#
#
#   global defined variables
#
#
# short dir name
#
#      root directory fo sub-dir : clip-prg
#                                  clip-devel
#                                  clip-new
#                                  clip-old
#                                  clip-install
#                                  cliproot
#                                  clip32
#                                  clip64
#                                  ..
#
#    Clip_H_Dir : clip host 						dir
#
#    Clip_M_Dir : clip main dir : clip-prg 	dir
#
#    Clip_B_Dir : clip-prg/bin  					dir
#
#    Clip_C_Dir : clip-prg/config 				dir
#
#    Clip_S_Dir : clip-prg/config/setup 		dir
#
#    Clip_T_Dir : clip-prg/temp 					dir
#
#
# short file name
#
#
#     Clip_F_F  : clip-prg/bin/functions 				file
#                 all common commands & functions
#
#
#     Clip_F_CFG : CLIPROOT/include/functions.cfg 		file
#                  contains declared variables needed for batch files
# 			clip_cp clip_makelib clip_makeslib clip_msgmerge
#			clip_msgfmt
#
export Parameter="$*"
#export MAKECMDGOALS=""
source config/functions.f
source config/gen.in
if [ $? != 0 ] ; then
	echo "Configuration for $PWD"
	error $PWD
	exit 1
fi

########################################################################################################"
#
OnScreen 5 "loading ..........."
#/sleep 1
#/more 	$Clip_M_Dir/AUTHORS 		\
#/		$Clip_M_Dir/README 			\
#/		$Clip_M_Dir/license.gnu
OnScreen 2
export varx=
#read -p "Press <ENTER> to continue" varx >&0
#beep_on
export >/dev/null
let trt=-1
export LANG=C
export LC_ALL=C
export LC_MESSAGES=C
export CLIP_NAMES=yes
export pwd=`pwd`
[ -f configure.ini ] && source configure.ini
while [ $trt -eq -1 ] ; do
	export ARCH=$(GetArch 0)
	OnScreen 0
	trt=0
	OnScreen 20
#/	sleep .5
	banner clip v.$release_version
#/	sleep .5
	banner R-$sequence_release
#/	sleep .5
	banner menu v.$Version
#/	sleep .5
	OnScreen 2
	cd $Clip_M_Dir
	export MenuNr=0
	Menu_Select[$MenuNr]="cancel      "
	Menu_command[$MenuNr]="cancel"
	      Menu2[$MenuNr]="                                                     |"
	let ++MenuNr
	Menu_Select[$MenuNr]="clean       "
	Menu_command[$MenuNr]="clean"
			Menu2[$MenuNr]=" : cleans all generated files                        |"
	export CleanNr=$MenuNr
	let ++MenuNr
	Menu_Select[$MenuNr]="cleand      "
	Menu_command[$MenuNr]="cleand"
			Menu2[$MenuNr]=" : forces clean because of errors                    |"
#	export CleanNr=$MenuNr
	let ++MenuNr
	Menu_Select[$MenuNr]="distclean   "
	Menu_command[$MenuNr]="distclean"
			Menu2[$MenuNr]=" : clean + installed files                           |"
	export DistcleanNr=$MenuNr
	export Separetor1=$MenuNr
	let ++MenuNr
	Menu_Select[$MenuNr]="new-config  "
	Menu_command[$MenuNr]="new-config"
			Menu2[$MenuNr]=" : cleans & changes your configuration settings      |"
	export new_configuration=$MenuNr
#	let ++MenuNr
#	Menu_Select[$MenuNr]="arch        "
#	Menu_command[$MenuNr]="arch"
#			Menu2[$MenuNr]=" : modifies arch                                     |"
#	export ArchService=$MenuNr
	export Separetor2=$MenuNr
	let ++MenuNr
	Menu_Select[$MenuNr]="local       "
	Menu_command[$MenuNr]="local"
			Menu2[$MenuNr]=" : local mode user only home_dir/bin                 |"
#	let ++MenuNr
#	Menu_Select[$MenuNr]="local-debug "
#			Menu2[$MenuNr]=" : local mode user only home_dir/bin with debug (-g) |"
	let ++MenuNr
	Menu_Select[$MenuNr]="system      "
	Menu_command[$MenuNr]="system"
			Menu2[$MenuNr]=" : /usr/local/clip                 NEEDS root access |"
			Menu2[$MenuNr]=" : /usr/local/clip                 NEEDS root access |"
#	let ++MenuNr
#	Menu_Select[$MenuNr]="system-debug"
#	Menu_command[$MenuNr]="system-debug"
#			Menu2[$MenuNr]=" : /usr/local/clip with debug (-g), NEEDS root access|"
	let ++MenuNr
	Menu_Select[$MenuNr]="opt         "
	Menu_command[$MenuNr]="opt"
			Menu2[$MenuNr]=" : /opt/clip                 NEEDS root access       |"
			Menu2[$MenuNr]=" : /opt/clip                 NEEDS root access       |"
#	let ++MenuNr
#	Menu_Select[$MenuNr]="opt-debug   "
#	Menu_command[$MenuNr]="opt-debug"
#			Menu2[$MenuNr]=" : /opt/clip with debug (-g), NEEDS root access      |"
	let ++MenuNr
	Menu_Select[$MenuNr]="log         "
	Menu_command[$MenuNr]="log"
			Menu2[$MenuNr]=" : view last log files                               |"
	export LogMenuNr=$MenuNr
	export Separetor3=$MenuNr
	let ++MenuNr
	Menu_Select[$MenuNr]="deb         "
	Menu_command[$MenuNr]="deb"
		Menu2[$MenuNr]=" : Debian package               !!! not yet tested   |"
	let ++MenuNr
	Menu_Select[$MenuNr]="rpm         "
	Menu_command[$MenuNr]="rpm"
		Menu2[$MenuNr]=" : rpm package    /usr          !!! not yet tested   |"
	let ++MenuNr
	Menu_Select[$MenuNr]="rpm-opt     "
	Menu_command[$MenuNr]="rpm-opt"
		Menu2[$MenuNr]=" : rpm package /opt             !!! not yet tested   |"
	let ++MenuNr
	Menu_Select[$MenuNr]="src         "
	Menu_command[$MenuNr]="src"
			Menu2[$MenuNr]="                                !!! not yet tested   |"
	let ++MenuNr
	Menu_Select[$MenuNr]="src_p       "
	Menu_command[$MenuNr]="src_p"
		Menu2[$MenuNr]="                                !!! not yet tested   |"
	let ++MenuNr
	Menu_Select[$MenuNr]="tar.bz2     "
	Menu_command[$MenuNr]="tar.bz2"
			Menu2[$MenuNr]="                                !!! not yet tested   |"
	let ++MenuNr
	Menu_Select[$MenuNr]="tbz         "
	Menu_command[$MenuNr]="tbz"
			Menu2[$MenuNr]="                                !!! not yet tested   |"
	let ++MenuNr
	Menu_Select[$MenuNr]="tbz2        "
	Menu_command[$MenuNr]="tbz2"
		Menu2[$MenuNr]="                                !!! not yet tested   |"
	let ++MenuNr
	Menu_Select[$MenuNr]="tgz         "
	Menu_command[$MenuNr]="tgz"
			Menu2[$MenuNr]="                                !!! not yet tested   |"
	let ++MenuNr
	Menu_Select[$MenuNr]="doc         "
	Menu_command[$MenuNr]="doc"
		Menu2[$MenuNr]="                                !!! not yet tested   |"
	let ++MenuNr
	Menu_Select[$MenuNr]="CVS_commit  "
	Menu_command[$MenuNr]="CVS_commit"
			Menu2[$MenuNr]="                                !!! not yet tested   |"
	let ++MenuNr
	Menu_Select[$MenuNr]="CVS_ucommit "
	Menu_command[$MenuNr]="CVS_ucommit"
			Menu2[$MenuNr]="                                !!! not yet tested   |"
	let ++MenuNr
	Menu_Select[$MenuNr]="CVS_update  "
	Menu_command[$MenuNr]="CVS_update"
			Menu2[$MenuNr]="                                !!! not yet tested   |"
	let ++MenuNr
	Menu_Select[$MenuNr]="depend      "
	Menu_command[$MenuNr]="depend"
			Menu2[$MenuNr]="                                !!! not yet tested   |"
	export Separetor4=0
	export Service=0
	if [ -f $Clip_H_Dir/clip-devel/config/make.release.sh ] ; then
		export Separetor4=$MenuNr
		let ++MenuNr
		Menu_Select[$MenuNr]="new-version "
		Menu_command[$MenuNr]="new-version"
			Menu2[$MenuNr]=" : modifies date &/or version &/or sequence          |"
		export Service=$MenuNr
	fi
	export varX=0
	export BigLineSeparator="#"
	export SpaceWhiteLine="#"
	export LittleLineSeparator="|"
	export nii=75
	while [ $nii -ge 0 ] ; do
		export BigLineSeparator="$BigLineSeparator#"
		export LittleLineSeparator="$LittleLineSeparator-"
		export SpaceWhiteLine="$SpaceWhiteLine "
		let --nii
	done
	export BigLineSeparator="$BigLineSeparator#"
	export LittleLineSeparator="$LittleLineSeparator|"
	export SpaceWhiteLine="$SpaceWhiteLine#"
	export SpaceReleaseVersion=""
	let nii=21
	while [ $nii -gt ${#release_version} ] ; do
		let --nii
		export SpaceReleaseVersion="$SpaceReleaseVersion "
	done
	export SpaceSequenceRelease=""
	let nii=21
	while [ $nii -gt ${#seq_no} ] ; do
		let --nii
		export SpaceSequenceRelease="$SpaceSequenceRelease "
	done
	let nii=58
	export SpaceWillUse=""
	while [ $nii -gt ${#MAKE} ] ; do
		let --nii
		export SpaceWillUse="$SpaceWillUse "
	done
	let nii=-1
	export SpaceInstaller=""
	export LenInstaller=$Version$ARCH
	export LenInstaller=${#LenInstaller}
	let nii=23
	while [ $nii -gt $LenInstaller ] ; do
		let --nii
		export SpaceInstaller="$SpaceInstaller "
	done
#/	sleep .5
	banner Arch
#/	sleep .5
	banner $ARCH
#/	sleep .5
	echo $BigLineSeparator 																													>&0
	echo "         Your system : CPU : $(uname -p)
                  CPU type : $(uname -m)
                        OS : $(uname -o)
                    kernel : $(uname -s)
                   release : $(uname -r)" 																							>&0
	echo $BigLineSeparator 																													>&0
#	echo "$SpaceWhiteLine" 																													>&0
	echo "#            Clip installer/Menu version : $Version for arch $ARCH$SpaceInstaller #" 						>&0
#	echo "$SpaceWhiteLine" 																													>&0
	echo $BigLineSeparator 																													>&0
#	echo "$SpaceWhiteLine" 																													>&0
	echo "#  Clip compiler, debugger, libraries & tools version : $release_version$SpaceReleaseVersion#" 			>&0
#	echo "#                                                date : $release_date           #"							>&0
	echo "#                                     version number  : $seq_no$SpaceSequenceRelease#" 			>&0
	echo "#                                                date : $sequence_date           #"							>&0
#	echo "$SpaceWhiteLine" 																													>&0
	echo $BigLineSeparator
#	echo "$SpaceWhiteLine" 																													>&0
	echo "#                  Menu options                                              #" 								>&0
#	echo "$SpaceWhiteLine" 																													>&0
	echo $BigLineSeparator
	yz=0
	while [ $yz -le $MenuNr ] ; do
		echo  "| [$yz] 	| ${Menu_Select[$yz]}  ${Menu2[$yz]}" 																	>&0
		case $yz in
		0)
#			echo $LittleLineSeparator 																										>&0
#			echo "|          Cleaning                                                          |" 						>&0
#			echo $LittleLineSeparator 																										>&0
			;;
		$Separetor1)
			echo $LittleLineSeparator 																										>&0
			echo "|          Versions                                                          |" 						>&0
			echo $LittleLineSeparator 																										>&0
			;;
		$Separetor2)
#			echo $LittleLineSeparator 																										>&0
			;;
		$Separetor3)
			echo $LittleLineSeparator 																										>&0
			echo "|          Packages                                                          |" 						>&0
			echo $LittleLineSeparator 																										>&0
			;;
		$Separetor4)
			echo $LittleLineSeparator 																										>&0
			echo "|          Service                                                           |" 						>&0
			echo $LittleLineSeparator 																										>&0
			;;
		esac
		let ++yz
	done
	echo $BigLineSeparator 																													>&0
#	echo "$SpaceWhiteLine" 																													>&0
	echo "#      will use : $MAKE $SpaceWillUse#"																					>&0
#	echo "$SpaceWhiteLine" 																													>&0

	echo $BigLineSeparator 																													>&0

	echo "      tested on Mandriva-Linux 2008.0 x86_64 / 2008.0 i586" 														>&0
#/
	read -p "				Your choice + <enter> : " varX 																			>&0
#/ -n 1

	if [[ $varX -gt $MenuNr ]] || [[ $varX -lt 0 ]] ; then
		let trt=-1
		continue
	fi
	if [[ $varX < "99" ]] && [ $varX -gt -1 ] ; then
		trt=$varX
		#export MAKECMDGOALS=${Menu_command[$varX]}
	else
		let trt=-1
	fi
	export >/dev/null
	if [ $trt -lt 0 ] || [ $trt -gt $MenuNr ] ; then
		beep_on
		let trt=-1
	elif [ $trt -eq 0 ] ; then
		exit 0
	elif [ $trt -eq $Service ] ; then
		[ -x ../clip-devel/config/make.release.sh ] && ../clip-devel/config/make.release.sh
		exec $0 $*
		let trt=-1
	elif [ $trt -eq $LogMenuNr ] ; then
		#[ -x viewlogfile.sh ] &&
		viewlogfile.sh
		let trt=-1
#	elif [[ $trt == $CleanNr ]] ; then
#		$MAKE ${Menu_command[$varX]}
#		if [ $? = 0 ] ; then
#			banner "$Parameter ${Menu_command[$varX]}  done."
#			read -p "Press <enter> to continue " varX
#		else
#			banner "$Parameter ${Menu_command[$varX]} error(s)"
#		fi
#		let trt=-1
#	elif [[ $trt == $DistcleanNr ]] ; then
#		$MAKE ${Menu_command[$varX]}
#		if [ $? = 0 ] ; then
#			banner "$Parameter ${Menu_command[$varX]}  done."
#			read -p "Press <enter> to continue " varX
#		else
#			banner "$Parameter ${Menu_command[$varX]} error(s)"
#		fi
#		let trt=-1
#	elif [ $trt -eq $ArchService ] ; then
#		rm -f$V $Clip_C_Dir/setup/*
#		ARCH=$(GetArch 1)
#		let trt=-1
#		if [[ $ARCH = "x86_64" ]] ; then
#			[ -d $Clip_H_Dir/clip64 ] || mkdir -p$V $Clip_H_Dir/clip64/include
#		fi
#		rm -Rf$V Makefile.ini Makefile configure.ini
#		source bin/init_Mkfile.sh
	elif [ $trt -eq $new_configuration ] ; then
		rm -Rf$V $Clip_S_Dir/*
		#rm -f$V $Clip_T_Dir/*
		rm -Rf$V Makefile.ini Makefile configure.ini
	fi
done


varX=$trt
OnScreen 5 "Starting $varX. ${Menu_Select[$varX]} command : $MAKE ${Menu_command[$varX]}"
OnScreen 5
if [[ "${Menu_Select[$varX]}" = system ]] ; then
	mkdir -p /usr/local/clip/doc || true
	touch /usr/local/clip/doc/tt || true
	if ! [ -f /usr/local/clip/doc/tt ] ; then
		deception
		OnScreen 1 "You have NO root access /usr/local/clip !!!!!!!!	"
		exit 1
	else
		rm -fv /usr/local/clip/doc/tt
		OnScreen 1 DO NOT forget to run clean as root
#		OnScreen 1 "or run 1st make user(-debug)" under normal user
#		OnScreen 1 "or run 1st make user" under normal user
		read -p "Press <ENTER> to continue" varx >&0
	fi
fi
if [[ "${Menu_Select[$varX]}" = opt ]] ; then
	mkdir -p /opt/clip/doc || true
	touch /opt/clip/doc/tt || true
	if ! [ -f /opt/clip/doc/tt ] ; then
		deception
		OnScreen 1 "You have NO root access to /opt/clip/ !!!!!!!!	"
		exit 1
	else
		rm -fv /usr/local/clip/doc/tt
		OnScreen 1 DO NOT forget to run clean as root
#		OnScreen 1 "or run 1st make user(-debug)" under normal user
#		OnScreen 1 "or run 1st make user" under normal user
		read -p "Press <ENTER> to continue" varx >&0
	fi
fi
export Command_line="$MAKE $Parameter ${Menu_command[$varX]} "
OnScreen 3
if [ -f $Clip_S_Dir/VerbosityLog.setup.ini ] ; then
	export VerbosityLog=$(cat $Clip_S_Dir/VerbosityLog.setup.ini)
else
	export VerbosityLog=-1
fi
OnScreen 1 "		Log file(s) :"
OnScreen 2 "	[ 1 ].	Start : $Command_line"
OnScreen 0 "	result will be store in the log file : $LogFileLog"
OnScreen 2 "	[ 2 ].	With separate files  : 2 files :"
OnScreen 0 "						$LogFileLog"
OnScreen 0 "						$LogFileErr"
OnScreen 2 "	[ 3 ].	NO log file	(on screen display)"
OnScreen 2 "							Advised [ 1 ]"
OnScreen 2 "	[ 0 ]		ABORT"
OnScreen 2
while [ $VerbosityLog -lt 0 ] || [ $VerbosityLog -gt 3 ]; do
	read -n 1 -p "		Your choice + <enter> : " VerbosityLog
done
OnScreen 3 "Starting $Command_line ________________________  clip"
OnScreen 1 "from $PWD"

OnScreen 2 "VerbosityLog : $VerbosityLog"
OnScreen 2 "Will execute command line : $Command_line "
date
rm -fv $Clip_M_Dir/temp/* >/dev/null
OnScreen 3 "	executing .............."
if [[ ${Menu_Select[$varX]} = "opt" ]] ; then
	export ReleaseDir="/opt"
elif [[ ${Menu_Select[$varX]} = "system" ]] ; then
	export ReleaseDir="/usr/local"
else
	export ReleaseDir="$Clip_H_Dir"
fi
#echo LogFileLog="$LogFileLog">>configure.ini
export >/dev/null
if [[ "$VerbosityLog" == "0" ]] ; then
	exit
fi
echo $VerbosityLog >$Clip_S_Dir/VerbosityLog.setup.ini
rm -f $Clip_T_Dir/*
export >/dev/null
[ -f Makefile ] || ./configure
[ -f configure.ini ] && source configure.ini
if [[ "$VerbosityLog" == "3" ]] ; then # previous 3
	OnScreen Menu 3
	OnScreen 3 "$Command_line			on screen"
#	$MAKE $Parameter hash2
	$Command_line
elif [[ "$VerbosityLog" = "1" ]] ; then
	OnScreen Menu 1
	rm -f$V $LogFileLog $LogFileErr
	OnScreen 3 "$Command_line to log file :"
	OnScreen 1 "						 1>$LogFileLog"
	OnScreen 1 "						 2>&1"
	OnScreen 1 "Log file : $LogFileLog"
#	$MAKE $Parameter hash2 1>>$LogFileLog 2>&1
#	$Command_line &1>$Clip_T_Dir/compile.log.$LogFileNr.sh 2>&1
	exec 									1>$LogFileLog
	exec 									2>$LogFileLog
	#$Command_line 1>>$LogFileLog 2>&1
	$Command_line
elif [[ "$VerbosityLog" = "2" ]] ; then
	OnScreen Menu 2
	rm -f$V $LogFileLog $LogFileErr
	OnScreen 3 "$Command_line to log files :"
	OnScreen 0 "						 2>$LogFileErr"
	OnScreen 0 "						 1>$LogFileErr"
	OnScreen 1 "Log files :	$LogFileLog"
	OnScreen 0 "					 $LogFileErr"
#	$MAKE $Parameter hash2 2>>$LogFileErr 1>$LogFileLog
	exec 									1>$LogFileLog
	exec 									2>$LogFileLog
	#$Command_line  2>$LogFileErr 1>$LogFileLog
	$Command_line
else
	OnScreen Menu $VerbosityLog
	$Command_line
fi
if [ $? = 0 ] ; then
	exec 									1>&-
	exec 									2>&-
	trumpet
	OnScreen 2
	banner "$Parameter ${Menu_command[$varX]}  done."
	OnScreen 2 ".......................done 1>&2"
	OnScreen 2 "::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::"
else
	exec 									1>&-
	exec 									2>&-
	deception
	OnScreen 2
	banner "$Parameter ${Menu_command[$varX]} error(s)"
fi
date
export >/dev/null
if [[ "$VerbosityLog" != "3" ]] ; then
	$Clip_C_Dir/viewlogfile.sh
fi
beep_on
beep_on
beep_on
