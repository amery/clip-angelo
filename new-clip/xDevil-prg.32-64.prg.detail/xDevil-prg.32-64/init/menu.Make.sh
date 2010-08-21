#!/bin/sh
#
#
[ -f config/user ] || echo $USER >config/user
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
#      root directory fo sub-dir : xDevil-prg
#                                  xDevil-devel
#                                  xDevil-new
#                                  xDevil-old
#                                  xDevil-install
#                                  xDevilroot
#                                  xDevil32
#                                  xDevil64
#                                  ..
#
#    xDevil_H_Dir : xDevil host 						dir
#
#    xDevil_M_Dir : xDevil main dir : xDevil-prg 	dir
#
#    xDevil_B_Dir : xDevil-prg/bin  					dir
#
#    xDevil_C_Dir : xDevil-prg/config 				dir
#
#    xDevil_S_Dir : xDevil-prg/config/setup 		dir
#
#    xDevil_T_Dir : xDevil-prg/temp 					dir
#
#
# short file name
#
#
#     xDevil_F_F  : xDevil-prg/bin/functions 				file
#                 all common commands & functions
#
#
#     xDevil_F_CFG : xDevilROOT/include/functions.cfg 		file
#                  contains declared variables needed for batch files
# 			xDevil_cp xDevil_makelib xDevil_makeslib xDevil_msgmerge
#			xDevil_msgfmt
#
export Parameter="$*"
#export MAKECMDGOALS=""
source init/functions.f.sh
export V="v"
#source init/gen.in
if [ $? != 0 ] ; then
	echo "Configuration for $PWD"
	error $PWD
	exit 1
fi
xDevil_M_Dir="$PWD"
if ! [ -f "$xDevil_M_Dir/release.date" ] ; then
	[ -x ../create.release.number.sh ] && ../create.release.number.sh
fi
export release_date=$(cat "$xDevil_M_Dir/release.date")
export release_version=""
if ! [ -f "$xDevil_M_Dir/release.version" ] ; then
	[ -x ../create.release.number.sh ] && ../create.release.number.sh
fi
export release_version=$(cat "$xDevil_M_Dir/release.version")
export release_sequence=""
if ! [ -f "$xDevil_M_Dir/release.sequence" ] ; then
	[ -x ../create.release.number.sh ] && ../create.release.number.sh
fi
export release_sequence=$(cat "$xDevil_M_Dir/release.sequence")
export seq_no=$release_sequence-$release_version
Version="1.1.01"
if [[ "$USER" != "root" ]] ; then
	echo $USER >"$xDevil_M_Dir/user"
fi
rm -f$V $xDevil_M_Dir/temp/*
########################################################################################################"
#
OnScreen 5 "loading ..........."
#/sleep 1
#/more 	$xDevil_M_Dir/AUTHORS 		\
#/		$xDevil_M_Dir/README 			\
#/		$xDevil_M_Dir/license.gnu
OnScreen 2
export varX=
#read -p "Press <ENTER> to continue" varX >&0
#beep_on
export >/dev/null
let trt=-1
export LANG=C
export LC_ALL=C
export LC_MESSAGES=C
export xDevil_NAMES=yes
export pwd=`pwd`
export WaitForCheck="Y"
source init/start.sh 1
let trt=-1
ARCH="$(uname -m)"
if ! [[ "$ARCH" = "x86_64" ]] ; then
	ARCH="i686"
fi
echo $ARCH >config/setup/ARCH.setup.ini

while [ $trt -eq -1 ] ; do
while [ $trt -eq -1 ] ; do
#	exec 									1>&-
#	exec 									2>&-
#	exec 									0>&-
	[ -d $HOME/xDevil/xDevil-prg.32-64.prg.detail/xDevil-prg.32-64 ] \
		&& cp -Rfpu$V $HOME/xDevil/xDevil-prg.32-64.prg.detail/xDevil-prg.32-64/* $xDevil_M_Dir/.
	export RootMode="0"
	export ARCH=$(GetArch 0)
	OnScreen 0
	trt=0
	OnScreen 20
#/	sleep .5
	banner xDevil
	banner Version
	banner $release_version
#/	sleep .5
	banner Release
	banner $release_sequence
#/	sleep .5
	banner menu
	banner v.$Version
#/	sleep .5
	OnScreen 2
	cd $xDevil_M_Dir
	export MenuNr=0
	Menu_Select[$MenuNr]="cancel/abort  "
	Menu_command[$MenuNr]="cancel"
	Menu2[$MenuNr]="                                                                |"
	let ++MenuNr
	Menu_Select[$MenuNr]="clean         "
	Menu_command[$MenuNr]="clean"
	Menu2[$MenuNr]=" : cleans all generated files                                   |"
	export CleanNr=$MenuNr
	let ++MenuNr
	Menu_Select[$MenuNr]="force.clean   "
	Menu_command[$MenuNr]="force.clean.sh"
	Menu2[$MenuNr]=" : forces clean because of errors                               |"
	export ForceCleanNr=$MenuNr
	let ++MenuNr
	Menu_Select[$MenuNr]="distclean     "
	Menu_command[$MenuNr]="distclean"
	Menu2[$MenuNr]=" : cleans config files + clean                                  |"
	export DistcleanNr=$MenuNr
	let ++MenuNr
	Menu_Select[$MenuNr]="uninstall     "
	Menu_command[$MenuNr]="uninstall"
	Menu2[$MenuNr]=" : cleans all installed files  + clean                          |"
	export UnInstallNr=$MenuNr
	export Separetor1=$MenuNr
	let ++MenuNr
	Menu_Select[$MenuNr]="new-config    "
	Menu_command[$MenuNr]="new-config"
	Menu2[$MenuNr]=" : cleans & changes your configuration settings                 |"
	export new_configuration=$MenuNr
	export Menu_indent=0
	let ++MenuNr
	Menu_Select[$MenuNr]="indent        "
	Menu_command[$MenuNr]="indent"
	Menu2[$MenuNr]=" : indents source files .c .h                                   |"
	export Menu_indent=$MenuNr
	export Separetor2=$MenuNr
	let ++MenuNr
	Menu_Select[$MenuNr]="depend        "
	Menu_command[$MenuNr]="depend"
	Menu2[$MenuNr]=" : checks dependencies of included files                        |"
	let ++MenuNr
	Menu_Select[$MenuNr]="log           "
	Menu_command[$MenuNr]="log"
	Menu2[$MenuNr]=" : view last log files                                          |"
	export LogMenuNr=$MenuNr
	export Separetor2=$MenuNr
	let ++MenuNr
	Menu_Select[$MenuNr]="local         "
	Menu_command[$MenuNr]="local"
	Menu2[$MenuNr]=" : local mode user only linked to home_dir/bin                  |"
	let ++MenuNr
	Menu_Select[$MenuNr]="home          "
	Menu_command[$MenuNr]="home"
	Menu2[$MenuNr]=" : home user only home_dir/xDevil(32/64) linked to home_dir/bin  |"
	let ++MenuNr
	Menu_Select[$MenuNr]="usr.local     "
	Menu_command[$MenuNr]="usr.local"
	Menu2[$MenuNr]=" : /usr/local/xDevil                          NEEDS root access  |"
	let ++MenuNr
	Menu_Select[$MenuNr]="opt           "
	Menu_command[$MenuNr]="opt"
	Menu2[$MenuNr]=" : /opt/xDevil(32/64)                         NEEDS root access  |"
	let ++MenuNr
	Menu_Select[$MenuNr]="sys           "
	Menu_command[$MenuNr]="sys"
	Menu2[$MenuNr]=" :                                       !!! not yet tested     |"
	export Separetor3=$MenuNr
	let ++MenuNr
	Menu_Select[$MenuNr]="deb           "
	Menu_command[$MenuNr]="deb"
	Menu2[$MenuNr]=" : Debian package                        !!! not yet tested     |"
	let ++MenuNr
	Menu_Select[$MenuNr]="rpm           "
	Menu_command[$MenuNr]="rpm"
	Menu2[$MenuNr]=" : rpm package    /usr                   !!! not yet tested     |"
	let ++MenuNr
	Menu_Select[$MenuNr]="rpm-opt       "
	Menu_command[$MenuNr]="rpm-opt"
	Menu2[$MenuNr]=" : rpm package /opt                      !!! not yet tested     |"
	let ++MenuNr
	Menu_Select[$MenuNr]="src           "
	Menu_command[$MenuNr]="src"
	Menu2[$MenuNr]=" :                                       !!! not yet tested     |"
	let ++MenuNr
	Menu_Select[$MenuNr]="src_p         "
	Menu_command[$MenuNr]="src_p"
	Menu2[$MenuNr]=" :                                       !!! not yet tested     |"
	let ++MenuNr
	Menu_Select[$MenuNr]="tar.bz2       "
	Menu_command[$MenuNr]="tar.bz2"
	Menu2[$MenuNr]=" :                                       !!! not yet tested     |"
	let ++MenuNr
	Menu_Select[$MenuNr]="tbz           "
	Menu_command[$MenuNr]="tbz"
	Menu2[$MenuNr]=" :                                       !!! not yet tested     |"
	let ++MenuNr
	Menu_Select[$MenuNr]="tbz2          "
	Menu_command[$MenuNr]="tbz2"
	Menu2[$MenuNr]=" :                                       !!! not yet tested     |"
	let ++MenuNr
	Menu_Select[$MenuNr]="tgz           "
	Menu_command[$MenuNr]="tgz"
	Menu2[$MenuNr]=" :                                       !!! not yet tested     |"
	let ++MenuNr
	Menu_Select[$MenuNr]="doc           "
	Menu_command[$MenuNr]="doc"
	Menu2[$MenuNr]=" :                                       !!! not yet tested     |"
	let ++MenuNr
	Menu_Select[$MenuNr]="CVS_commit    "
	Menu_command[$MenuNr]="CVS_commit"
	Menu2[$MenuNr]=" :                                       !!! not yet tested     |"
	let ++MenuNr
	Menu_Select[$MenuNr]="CVS_ucommit   "
	Menu_command[$MenuNr]="CVS_ucommit"
	Menu2[$MenuNr]=" :                                       !!! not yet tested     |"
	let ++MenuNr
	Menu_Select[$MenuNr]="CVS_update    "
	Menu_command[$MenuNr]="CVS_update"
	Menu2[$MenuNr]=" :                                       !!! not yet tested     |"
	export Separetor4=0
	export varX=0
	export BigLineSeparator="#"
	export SpaceWhiteLine="#"
	export LittleLineSeparator="|"
	export nii=88
	while [ $nii -ge 0 ] ; do
		export BigLineSeparator="$BigLineSeparator#"
		export LittleLineSeparator="$LittleLineSeparator-"
		export SpaceWhiteLine="$SpaceWhiteLine "
		let --nii
	done
	export BigLineSeparator="$BigLineSeparator#"
	export LittleLineSeparator="$LittleLineSeparator|"
	export SpaceWhiteLine="$SpaceWhiteLine#"
	export SpaceInstaller=""
	export LenInstaller="$Version$ARCH"
	export LenInstaller=${#LenInstaller}
	let nii=33
	while [ $nii -gt $LenInstaller ] ; do
		let --nii
		export SpaceInstaller="$SpaceInstaller "
	done
	export SpaceReleaseVersion=""
	let nii=30
	while [ $nii -gt ${#release_version} ] ; do
		let --nii
		export SpaceReleaseVersion="$SpaceReleaseVersion "
	done
	export SpaceReleaseSequence=" "
	let nii=37
	while [ $nii -gt ${#seq_no} ] ; do
		let --nii
		export SpaceReleaseSequence="$SpaceReleaseSequence "
	done
	let nii=69
	export SpaceWillUse=""
	while [ $nii -gt ${#MAKE} ] ; do
		let --nii
		export SpaceWillUse="$SpaceWillUse "
	done
#/	sleep .5
	beep_on
	banner Arch
#/	sleep .5
	banner $ARCH
	sleep .5
	echo $BigLineSeparator 																													>&0
	echo "         Your system : CPU : $(uname -p)
                  CPU type : $(uname -m)
                        OS : $(uname -o)
                    kernel : $(uname -s)
                   release : $(uname -r)" 																							>&0
	echo $BigLineSeparator 																													>&0
#	echo "$SpaceWhiteLine" 																													>&0
	echo "#   xDevil installer/Menu version : $release_sequence-$Version for arch $ARCH$SpaceInstaller   #" 			>&0
#	echo "$SpaceWhiteLine" 																													>&0
	echo $BigLineSeparator 																													>&0
#	echo "$SpaceWhiteLine" 																													>&0
	echo "#  xDevil compiler, debugger, libraries & tools version : $release_version$SpaceReleaseVersion   #" 		>&0
	echo "#                                   (yyyy-mm-dd)  date : $release_date                       #"			>&0
	echo "#                                     release sequence : $release_sequence$SpaceReleaseSequence       #" 	>&0
#	echo "#                                                date : $sequence_date           #"							>&0
#	echo "$SpaceWhiteLine" 																													>&0
	echo $BigLineSeparator
#	echo "$SpaceWhiteLine" 																													>&0
	echo "#                  Menu options                                                           #" 				>&0
#	echo "$SpaceWhiteLine" 																													>&0
	echo $BigLineSeparator
	yz=0
	while [ $yz -le $MenuNr ] ; do
		echo  "| [$yz] 	| ${Menu_Select[$yz]}  ${Menu2[$yz]}" 																	>&0
		case $yz in
		0)
			;;
		$Separetor1)
			echo $LittleLineSeparator 																										>&0
			echo "|          Tools                                                                          |" 		>&0
			echo $LittleLineSeparator 																										>&0
			;;
		$Separetor2)
			echo $LittleLineSeparator 																										>&0
			echo "|          Versions                                                                       |" 		>&0
			echo $LittleLineSeparator 																										>&0
			;;
		$Separetor3)
			echo $LittleLineSeparator 																										>&0
			echo "|          Packages                                                                       |" 		>&0
			echo $LittleLineSeparator 																										>&0
			;;
		esac
		let ++yz
	done
	echo $BigLineSeparator 																													>&0
#	echo "$SpaceWhiteLine" 																													>&0
	echo "#      will use : $MAKE $SpaceWillUse  #"																					>&0
#	echo "$SpaceWhiteLine" 																													>&0

	echo $BigLineSeparator 																													>&0

	echo "tested on : see : http://users.skynet.be/angelo_girardi/it/download/systems.html" 							>&0
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
#	elif [ $trt -eq $Service ] ; then
#		cd ..
#		[ -x ./create.release.number.sh ] && ./create.release.number.sh
#		cd "$xDevil_M_Dir"
#		exec $0 $*
#		let trt=-1
	elif [ $trt -eq $LogMenuNr ] ; then
		#[ -x viewlogfile.sh ] &&
		init/viewlogfile.sh
		let trt=-1
	elif [ $trt -eq $ForceCleanNr ] ; then
		./force.clean.sh
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
#		rm -f$V $xDevil_C_Dir/setup/*
#		ARCH=$(GetArch 1)
#		let trt=-1
#		if [[ $ARCH = "x86_64" ]] ; then
#			[ -d $xDevil_H_Dir/xDevil64 ] || mkdir -p$V $xDevil_H_Dir/xDevil64/include
#		fi
#		rm -Rf$V Makefile.ini Makefile configure.ini
#		source bin/init_Mkfile.sh
	elif [ $trt -eq $UnInstallNr ] ; then
		varX=-1
		while [[ $varX -lt 0 ]] || [[ $varX -gt 4 ]] ; do
			beep_on
			OnScreen 10
			echo "#####################################"
			echo "#   [0]  |  cancel/abort            #"
			echo "#####################################"
			echo "#   [1]  |  Uninstall 'local'       #"
			echo "#   [2]  |  Uninstall 'home'        #"
			echo "#   [3]  |  Uninstall 'usr.local'   #"
			echo "#   [4]  |  Uninstall 'opt'         #"
#			echo "#   [5]  |  Uninstall 'sys'"        #
			echo "#####################################"
			echo ""
			read -p "Your choice + <enter> : " varX >&0
		done
		if [[ $varX -eq 0 ]] ; then
			exit 0
		elif [[ $varX -eq 1 ]] ; then
			[ -f Makefile ] || ./configure "clean"
			$MAKE clean
			rm -Rf$V $xDevil_H_Dir/xDevil32 || true
			rm -Rf$V $xDevil_H_Dir/xDevil64 || true
			rm -Rf$V $xDevil_H_Dir/ci_xDevil32 || true
			rm -Rf$V $xDevil_H_Dir/ci_xDevil64 || true
		elif [[ $varX -eq 2 ]] ; then
			[ -f Makefile ] || ./configure "clean"
			$MAKE clean
		elif [[ $varX -eq 3 ]] ; then
			[ -f Makefile ] || ./configure "clean"
			$MAKE clean
			su -c "(rm -Rf$V /usr/local/xDevil32 || true) && \
					(rm -Rf$V /usr/local/xDevil64 || true) && \
					(rm -Rf$V /usr/local/ci_xDevil32 || true) && \
					(rm -Rf$V /usr/local/ci_xDevil64 || true)"
		elif [[ $varX -eq 4 ]] ; then
			[ -f Makefile ] || ./configure "clean"
			$MAKE clean
			su -c "(rm -Rf$V /opt/xDevil32 || true) && \
					(rm -Rf$V /opt/xDevil64 || true) && \
					(rm -Rf$V /opt/ci_xDevil32 || true) && \
					(rm -Rf$V /opt/ci_xDevil64 || true)"
#		elif [[ $varX -eq 5 ]] ; then
#			[ -f Makefile ] || ./configure "clean"
#			$MAKE clean
#			su -c "(rm -Rf$V $xDevil_H_Dir/xDevil32 || true) && \
#					(rm -Rf$V $xDevil_H_Dir/xDevil64 || true )&& \
#					(rm -Rf$V $xDevil_H_Dir/ci_xDevil32 || true) && \
#					(rm -Rf$V $xDevil_H_Dir/ci_xDevil64 || true)"
		fi
		rm -Rf$V $HOME/xDevil32 || true
		rm -Rf$V $HOME/xDevil64 || true
		rm -Rf$V $HOME/ci_xDevil32 || true
		rm -Rf$V $HOME/ci_xDevil64 || true
		trumpet
		exit 0
	elif [ $trt -eq $new_configuration ] ; then
		rm -Rf$V $xDevil_S_Dir/*
		#rm -f$V $xDevil_T_Dir/*
		rm -Rf$V Makefile.ini Makefile configure.ini
	fi
done


varX=$trt
OnScreen 5 "Starting $varX. ${Menu_Select[$varX]} command : $MAKE ${Menu_command[$varX]}"
OnScreen 5
if [[ "${Menu_Select[$varX]}" = "usr.local" ]] ; then
	mkdir -p /usr/local/xDevil/doc || true
	touch /usr/local/xDevil/doc/tt
	if [[ $? != 0 ]] ; then
		deception
		OnScreen 1 "You have NO root access /usr/local/xDevil !!!!!!!!	"
		RootMode="1"
	else
		rm -fv /usr/local/xDevil/doc/tt
#		OnScreen 1 DO NOT forget to run clean as root
#		OnScreen 1 "or run 1st make user(-debug)" under normal user
#		OnScreen 1 "or run 1st make user" under normal user
#		read -p "Press <ENTER> to continue" varX >&0
	fi
fi
if [[ "${Menu_Select[$varX]}" = "opt" ]] ; then
	mkdir -p /opt/xDevil/doc || true
	touch /opt/xDevil/doc/tt
	if [[ $? != 0 ]] ; then
		deception
		OnScreen 1 "You have NO root access to /opt/xDevil/ !!!!!!!!	"
		RootMode="1"
	else
		rm -fv /usr/local/xDevil/doc/tt
#		OnScreen 1 DO NOT forget to run clean as root
#		OnScreen 1 "or run 1st make user(-debug)" under normal user
#		OnScreen 1 "or run 1st make user" under normal user
#		read -p "Press <ENTER> to continue" varX >&0
	fi
fi
export Command_line="$MAKE $Parameter ${Menu_command[$varX]} "
OnScreen 3
if [ -f $xDevil_S_Dir/VerbosityLog.setup.ini ] ; then
	export VerbosityLog=$(cat $xDevil_S_Dir/VerbosityLog.setup.ini)
else
	export VerbosityLog=-1
fi
#OnScreen 1 "		Log file(s) :"
#OnScreen 2 "	[ 1 ].	Start : $Command_line"
#OnScreen 0 "	result will be store in the log file : $LogFileLog"
#OnScreen 2 "	[ 2 ].	With separate files  : 2 files :"
#OnScreen 0 "						$LogFileLog"
#OnScreen 0 "						$LogFileErr"
#OnScreen 2 "	[ 3 ].	NO log file	(on screen display)"
#OnScreen 2 "							Advised [ 1 ]"
#OnScreen 2 "	[ 0 ]		ABORT"
#OnScreen 2
#while [ $VerbosityLog -lt 0 ] || [ $VerbosityLog -gt 3 ]; do
#	read -n 1 -p "		Your choice + <enter> : " VerbosityLog
#done
OnScreen 3 "Starting $Command_line ________________________  xDevil"
OnScreen 1 "from $PWD"

#OnScreen 2 "VerbosityLog : $VerbosityLog"
#OnScreen 2 "Will execute command line : $Command_line "
date
rm -fv $xDevil_M_Dir/temp/* >/dev/null
OnScreen 3 "	executing .............."
if [[ ${Menu_Select[$varX]} = "opt" ]] ; then
	export ReleaseDir="/opt"
elif [[ ${Menu_Select[$varX]} = "usr.local" ]] ; then
	export ReleaseDir="/usr/local"
elif [[ ${Menu_Select[$varX]} = "home" ]] ; then
	export ReleaseDir="$HOME"
else
	export ReleaseDir="$xDevil_H_Dir"
fi
#echo LogFileLog="$LogFileLog">>configure.ini
export >/dev/null
#if [[ "$VerbosityLog" == "0" ]] ; then
#	exit
#fi
echo $VerbosityLog >$xDevil_S_Dir/VerbosityLog.setup.ini
rm -f $xDevil_T_Dir/*
if [ -f ./configure.ini ] ; then
	source ./configure.ini
fi
export >/dev/null
if [[ -f ./${Menu_command[$varX]} ]] ; then
	rm -f$V ./${Menu_command[$varX]}
fi
if [[ -f ./${Menu_command[$varX]}.sh ]] ; then
	if [[ "$RootMode" = "1" ]] ; then
		su -c "./${Menu_command[$varX]}.sh"
	else
		./${Menu_command[$varX]}.sh
		if [[ $? != 0 ]] ; then
		 ErrorLevel=1
	  fi
	fi
else
	[ -f Makefile ] || ./configure ${Menu_Select[$varX]}
#if [[ "$VerbosityLog" == "3" ]] ; then # previous 3
#	OnScreen Menu 3
#	OnScreen 3 "$Command_line			on screen"
#	$MAKE $Parameter hash2
#	$Command_line
#elif [[ "$VerbosityLog" = "1" ]] ; then
#	OnScreen Menu 1
#	rm -f$V $LogFileLog $LogFileErr
#	OnScreen 3 "$Command_line to log file :"
#	OnScreen 1 "						 1>$LogFileLog"
#	OnScreen 1 "						 2>&1"
#	OnScreen 1 "Log file : $LogFileLog"
#	$MAKE $Parameter hash2 1>>$LogFileLog 2>&1
#	$Command_line &1>$xDevil_T_Dir/compile.log.$LogFileNr.sh 2>&1
#	exec 									1>$LogFileLog
#	exec 									2>$LogFileLog
#	#$Command_line 1>>$LogFileLog 2>&1
#	$Command_line
#elif [[ "$VerbosityLog" = "2" ]] ; then
#	OnScreen Menu 2
#	rm -f$V $LogFileLog $LogFileErr
#	OnScreen 3 "$Command_line to log files :"
#	OnScreen 0 "						 2>$LogFileErr"
#	OnScreen 0 "						 1>$LogFileErr"
#	OnScreen 1 "Log files :	$LogFileLog"
#	OnScreen 0 "					 $LogFileErr"
#	$MAKE $Parameter hash2 2>>$LogFileErr 1>$LogFileLog
#	exec 									1>$LogFileLog
#	exec 									2>$LogFileLog
#	#$Command_line  2>$LogFileErr 1>$LogFileLog
#	$Command_line
#else
#	OnScreen Menu $VerbosityLog
	$Command_line
fi
if [ $? = 0 ] && [ -f $xDevil_M_Dir/config/compile.ok ] ; then
#	exec 									1>&-
#	exec 									2>&-
	trumpet
	OnScreen 2
	banner ".$Parameter"
	banner "${Menu_command[$varX]}"
	banner "done."
	OnScreen 2 ".......................done 1>&2"
	OnScreen 2 "::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::"
else
#	exec 									1>&-
#	exec 									2>&-
	deception
	OnScreen 2
	banner ".$Parameter"
	banner "${Menu_command[$varX]}"
	banner "error(s)"
	sleep 2
	$xDevil_M_Dir/init/viewlogfile.sh
fi
#date
#export >/dev/null
#if [[ "$VerbosityLog" != "3" ]] ; then
#fi
#beep_on
#beep_on
#beep_on
let trt=-1
done
