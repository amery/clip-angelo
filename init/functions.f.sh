#!/bin/bash
#
############################################################################################################
############################################################################################################
############################################################################################################
##                                                                                                        ##
##          This file contains all the functions called in the configure files.                           ##
##                                                                                                        ##
##                                                                                                        ##
############################################################################################################
############################################################################################################
############################################################################################################
##                                                                                                        ##
##   Configure script from old clip                                                                       ##
##   Paul Lasarev - 2000.                                                                                 ##
##                                                                                                        ##
##                                                                                                        ##
##                                                                                                        ##
##   modified by Angelo GIRARDI                                                                           ##
##      <angelo.girardi@swing.be>                                                                         ##
##      http://users.skynet.be/sb172624                                                                   ##
##                                                                                                        ##
##   1st release on 01/2005 to integrate all configure(s),                                                ##
##                                                                                                        ##
##                                                                                                        ##
############################################################################################################
############################################################################################################
##                                                                                                        ##
##   this script CANNOT be run alone, it MUST be called from ../configure or ../clip_make.sh              ##
##                                                                                                        ##
############################################################################################################
############################################################################################################
#ProgName=""
#c_files=""
#ch_files=""
#h_files=""
#prg_files=""
#OBJ_files=""
export StartTime=$(date +%s)
export StartTotal=1
export Total=1
export TotalDone=1
export TotalRemaining=1
export Bban=1
let Delay=0
export Delay
#ADDOBJS=""
#BIN=""

############################################################################################################
############################################################################################################
##                                                                                                        ##
##                                                                                                        ##
##         function's section                                                                             ##
##                                                                                                        ##
##                                                                                                        ##
##         alphaorder classified                                                                          ##
##                                                                                                        ##
############################################################################################################
############################################################################################################

function allConfigure ()
	{
		#
		# scans all directories & runs configure
		#
		local ListFile
		local nFile
		local CurDir="$Clip_M_Dir/$*"
		cd "$CurDir"
		OnScreen 1 "Configuration for $PWD"
		export >/dev/null
		[ -f $CONFIGURE_SH ] && $CONFIGURE_SH $MAKECMDGOALS
		[ $? != 0 ] && error $CurDir
	}

function AllExpanded ()
	{
		local lWord=$1
		local lString=
		local aString=""
		local aPos=""
		local ReturnString=""
		local lExtension=$2
		local nii
		local njj
		local nkk
		local nmm
		local sii
		local sjj
		local lenPar
		local nPos
		local nChar
		nChar=${#aAllCharacter[@]}
		nPos=0
		lenPar=${#lWord}
		lString=""
		nii=0
		# echo will expand $* >&0
		while [ $nii -lt $lenPar ] ; do
		aString[$nii]=${aAllCharacter[0]}
			lString="$lString${aString[$nii]}"
			aPos[$nii]=0
			let ++nii
		done
		let njj=1
		#echo ${aString[@]}
		#echo $lString
		nPos=0
		nii=0
		nt=0
		while [[ $lString$lExtension != $lWord ]] && [ $nii -lt $lenPar ] ; do
			#nPos=0
			while [[ $lString$lExtension  < $lWord ]] && [ $nPos -lt $nChar ] ; do
				aPos[$nii]=$nPos
				nt=$nii
				let ++nPos
				aString[$nii]=${aAllCharacter[$nPos]}
				lString=""
				nmm=0
				while [ $nmm -le $nii ] ; do
					lString="$lString${aString[$nmm]}"
					let ++nmm
				done
			done
			if [[ $lString$lExtension = $lWord ]] ; then
				continue
			fi
			if [ $nPos -lt $nChar ] ; then
				nPos=${aPos[$nii]}
				aString[$nii]=${aAllCharacter[$nPos]}
				let ++nii
				aString[$nii]=${aAllCharacter[0]}
				aPos[$nii]=0
				nPos=0
				lString=""
				nmm=0
				while [ $nmm -le $nii ] ; do
					lString="$lString${aString[$nmm]}"
					let ++nmm
				done
			fi
			if [[ $lString$lExtension  > $lWord ]] ; then
				while [[ $lString$lExtension  > $lWord ]] && [ $nPos -lt $nChar ] && [ $nii -lt $lenPar ] ; do
					aPos[$nii]=$nPos
					let ++nPos
					aString[$nii]=${aAllCharacter[$nPos]}
					nmm=0
					lString=""
					while [ $nmm -le $nii ] ; do
						lString="$lString${aString[$nmm]}"
						let ++nmm
					done
				done
			fi
			if [ $nPos -gt $nChar ] ; then
				let --nii
				nPos=${aPos[$nii]}
				let ++nPos
				aPos[$nii]=$nPos
				aString[$nii]=${aAllCharacter[$nPos]}
				let ++nii
				nPos=0
				aPos[$nii]=$npos
				aString[$nii]=${aAllCharacter[$nPos]}
			fi
			lString=""
			nmm=0
			while [ $nmm -le $nii ] ; do
				lString="$lString${aString[$nmm]}"
				let ++nmm
			done
		done
		echo $lString
	}
# end function AllExpanded

function AllLower ()
	{
		local x=$1
		local y
		y=$(echo $x | tr "[:upper:]" "[:lower:]" )
		echo $y
	}
# end function AllLower

function AllUpper ()
	{
		local x="$*"
		local y
		y="$(echo $x | tr "[:lower:]" "[:upper:]" )"
		echo $y
	}
# end function AllLower
function arch_sh ()
	{
		a="$HOSTTYPE"
		[ -z "$a" ] && a=`uname -m 2>/dev/null`
		[ -z "$a" ] && a=`uname -p 2>/dev/null`
		[ -z "$a" ] && a=unknown
		case "$a" in
		i?86)
			echo i386
			;;
		*)
			echo $a
			;;
		esac
}
# end function arch_sh

function banner ()
	{
		if [[ "$Bban" != 0 ]] ; then
			export Bban=0
			Check_Bin banner xyz clip yes /usr/bin banner-xx-$ARCH.rpm
		fi
		if [ -x /usr/bin/banner ] ; then
			/usr/bin/banner $* 	>&0
		else
			echo $*					>&0
		fi
	}
# end function banner

function beep_on ()
	{
		local CurTime=$(date +%s)
		local n01ii=0
		let n01ii=$CurTime-$Delay
		if [[ $n01ii -gt 30 ]] ; then
			Delay=$(date +%s)
#			if [ -x /usr/bin/beep ] ;then
#				echo " beep beep" 	>/dev/null
#				/usr/bin/beep -f50 -l 200 -d 100 - e/dev/audio
#			elif [ -x /usr/bin/gnubeep ] ;then
#				echo " beep beep" 	>/dev/null
#				/usr/bin/gnubeep -b
#			el
			if [ -x /usr/bin/mplayer ] ; then
				/usr/bin/mplayer -vo null $Clip_M_Dir/sounds/Kopete_Received.ogg 1>/dev/null 2>/dev/null
				echo " beep mplayer beep" 	>/dev/null
				echo "" 					>&0
			elif [ -x /usr/bin/play ] ; then
				/usr/bin/play -ao alsa $Clip_M_Dir/sounds/Kopete_Received.ogg 1>/dev/null 2>/dev/null
				echo " beep play beep" 	>/dev/null
				echo "" 					>&0
			else
				echo " beep beep" 	>/dev/null
				echo "" 					>&0
			fi
		fi

	}
# end function beep_on

function Check_Bin ()
	{
		#
		# verifies if the asked file exists
		#
		local LogFile="$Clip_T_Dir/askbin.log"
		local WhatToFind
		local Extension
		local RequestedBy
		local Important
		local WhereToFind
		local Found=""
		local PackName
		local lList
		local fName
		local ArrayToFind
		local IdxToFind
		local CurIdx
		local Comment
		local CommentNr
		local varX="N"
		unset ArrayToFind
		SleepTime=0
		# sleep $SleepTime
		WhatToFind=$1
		shift
		# sleep $SleepTime
		Extension=$1
		shift
		# sleep $SleepTime
		RequestedBy=$1
		shift
		# sleep $SleepTime
		Important=$1
		Important=$(AllUpper $Important )
		# sleep $SleepTime
		#OnScreen 0 "Working dir :" $PWD
		if [[ $Important = YES ]] || [[ $Important = NO ]] ; then
			echo "" >/dev/null
		else
			error $*
		fi
		shift
		WhereToFind=$1
		# sleep $SleepTime
		shift
		PackName=""
		let CommentNr=-1
		IdxToFind=false
		while [ -n "$*" ] ; do
			if [ "$1" = "/" ] ; then
				IdxToFind=true
			elif  [ $IdxToFind = true ] ; then
				let ++CommentNr
				Comment[$CommentNr]=$1
			else
				PackName="$PackName $1"
			fi
			shift
		done
		# sleep $SleepTime
		OnScreen 0
		if [ -z $WhatToFind ] ; then
			error "No parameter passed in calling function Check_Bin"
		fi
		IdxToFind=0
		if [ -f "$Clip_S_Dir/$ARCH/$WhatToFind/.set" ] ; then
			CurIdx="$Clip_S_Dir/$ARCH/$WhatToFind/.set"
			lList="$(cat $CurIdx)"
			# sleep $SleepTime
			for fName in $lList ; do
				if [[ x"$fName" != x"" ]] ; then
					ArrayToFind[$IdxToFind]=$fName
					OnScreen 2 "Found : $WhatToFind requested by : $RequestedBy found in : $fName"
					echo $fName >"$Clip_S_Dir/$ARCH/$WhatToFind/.set"
					if [ -f $fName/$WhatToFind ] ; then
						echo $fName
						return
				  	fi
					let ++IdxToFind
					# sleep $SleepTime
				fi
			done
		fi
		if [ -n $WhereToFind ] ; then
			ArrayToFind[$IdxToFind]=$WhereToFind
			let ++IdxToFind
			# sleep $SleepTime
		fi
		ArrayToFind[$IdxToFind]=/usr/bin
		let ++IdxToFind
		# sleep $SleepTime
		ArrayToFind[$IdxToFind]=/sbin
		let ++IdxToFind
#		# sleep $SleepTime
		ArrayToFind[$IdxToFind]=/usr/sbin
		let ++IdxToFind
#		# sleep $SleepTime
		ArrayToFind[$IdxToFind]=/usr/local
		let ++IdxToFind
		ArrayToFind[$IdxToFind]=/opt
		let ++IdxToFind
#		# sleep $SleepTime
		ArrayToFind[$IdxToFind]=/usr
		let ++IdxToFind
#		# sleep $SleepTime
		ArrayToFind[$IdxToFind]=/opt
		if [[ "$WaitForCheck" = [Yy] ]] ; then
			let ++IdxToFind
			ArrayToFind[$IdxToFind]=/
		fi
		if [[ $Extension != xyz ]] ; then
			WhatToFindTmp=$WhatToFind$Extension
		fi
		while [[ $varX = [nN] ]] ; do
			CurIdx=0
			while [ -z "$Found" ] && [ $CurIdx -le $IdxToFind ] ; do
				#OnScreen CurIdx : $CurIdx IdxToFind : $IdxToFind ArrayLen : ${#ArrayToFind[@]} contents : ${ArrayToFind[@]}
				#OnScreen CurIdx : $CurIdx IdxToFind : $IdxToFind Seach in : ${ArrayToFind[$CurIdx]}
				WhereToFind=${ArrayToFind[$CurIdx]}
				# sleep $SleepTime
				if [[ "$WhereToFind" = "/" ]] ; then
					banner "Not Found"
					OnScreen 1 "in : ${ArrayToFind[@]}"
					OnScreen 2
					tptptpt=0
					while [ $tptptpt -le $CommentNr ] ; do
						OnScreen 0 ${Comment[$tptptpt]}
						let ++tptptpt
					done
					beep_on
					OnScreen 2 " Scan the all s[Y]stem : '$WhereToFind'"
					OnScreen 0 " or install package : $PackName"
					OnScreen 0 " and [R]escan standard directories"
					OnScreen 0 "${ArrayToFind[@]}"
					OnScreen 0 " or [N] skip : "
					OnScreen 2
					varX=" "
					read -n 1 -p "Your choice : [Y/N/R] : " varX 	>&0
					if [[ $varX = [nN] ]] ; then
						rm -f$V ../$RequestedBy.ok	>&0
						touch ../$RequestedBy.no
						let ++CurIdx
						continue
					elif [[ $varX = [rR] ]] ; then
						CurIdx=0
						continue
					elif [[ $varX = [Yy] ]] ; then
						OnScreen 0
					else
						let --CurIdx
						continue
					fi
				fi
				#OnScreen 0 Working dir : $PWD
				OnScreen 0 "Searching for : $WhatToFind in : $WhereToFind requested by : $RequestedBy"
				# sleep $SleepTime
				if [ -f $WhereToFind/$WhatToFind ] ; then
					Found=$WhereToFind
					# sleep $SleepTime
				else
					Found=$(Check_Find $WhereToFind $WhatToFind )
					# sleep $SleepTime
				fi
				let ++CurIdx
			done
			# sleep $SleepTime
			if [ -z "$Found" ] ; then
				varX="Y"
				banner "Missing" 																											>&0
				OnScreen 0 "Missing : $WhatToFind requested by : $RequestedBy Package name(s) : $PackName"
				echo "Missing : $WhatToFind requested by : $RequestedBy Package name(s) : $PackName" 				>&2
				# sleep $SleepTime
				if [[ $Important = YES ]] ; then
					echo "#" 																		>>$LogFile
					echo "This package is REQUIRED" 											>>$LogFile
					echo "#"																			>>$LogFile
					# sleep $SleepTime
				fi
				echo "Missing : $WhatToFind requested by : $RequestedBy"				>>$LogFile
				for IdxToFind in $PackName ; do
					echo "Package name(s) : $IdxToFind" 									>>$LogFile
				done
				rm -f$V ../$RequestedBy.ok 	>&0
				touch ../$RequestedBy.no
			else
				beep_on
				OnScreen 2 "Found : $WhatToFind requested by : $RequestedBy found in : $Found"
				OnScreen 2 "[s]earch path; is it Ok [y/n/s] : "
				read -n 1 varX 	>&0
				OnScreen 2
				if [[ $varX = [yY] ]] ; then
					mkdir -p$V "$Clip_S_Dir/$ARCH/$WhatToFind/" >&0
					echo $Found >"$Clip_S_Dir/$ARCH/$WhatToFind/.set"
					# sleep $SleepTime
					# OnScreen 0
					if [[ "$Found/$WhatToFind" -nt Makefile.in ]] ; then
						touch Makefile.in
					fi
					echo $Found
					# sleep $SleepTime
				elif [[ $varX = [sS] ]] ; then
					OnScreen 1 "Search path : "
					read -p varX 	>&0
					ArrayToFind[0]=$varX
					varX="N"
					Found=""
				else
					rm -f ../$RequestedBy.ok 	>&0
					touch ../$RequestedBy.no 	>&0
					Found=""
					echo "z" 	>&0
					varX="N"
				fi
			fi
		done
	}
# end function Check_Bin

function Check_Find ()
	{
		#
		# returns the 1st found directory
		#
		local WhereToFind=$1*
		local WhatToFind=$2
		local Found
		local HaveFound
		local nii

		HaveFound=$(find $WhereToFind -name $WhatToFind -printf "%h " 2>/dev/null)
		Found=""
		for nii in $HaveFound ; do
			if [ -n $Found ] ; then
				Found=$nii
			fi
		done
		echo $Found
	}
# end function Check_Find

function Check_Include ()
	{
		#
		# verifies if the asked 'include' file exists
		#
		local LogFile="$Clip_T_Dir/askinclude.log"
		local WhatToFind
		local Extension
		local RequestedBy
		local VariableToCreate
		local Important
		local WhereToFind
		local Found=""
		local PackName
		local ArrayToFind
		local IdxToFind
		local CurIdx
		local Comment
		local CommentNr
		local varX="N"
		IdxToFind=0
		unset ArrayToFind
		WhatToFind=$1
		shift
		Extension=$1
		shift
		RequestedBy=$1
		shift
		Important=$1
		Important=$(AllUpper $Important )
		#OnScreen 0 "Working dir :" $PWD
		if [[ "$Important" = YES  ]] || [[ "$Important" = NO ]] ; then
			echo "" >/dev/null
		else
			error $*
		fi
		shift
		WhereToFind=$1
		shift
		PackName=""
		let CommentNr=-1
		IdxToFind=false
		while [ -n "$*" ] ; do
			if [ "$1" = "/" ] ; then
				IdxToFind=true
			elif  [ $IdxToFind = true ] ; then
				let ++CommentNr
				Comment[$CommentNr]=$1
			else
				PackName="$PackName $1"
			fi
			shift
		done
		OnScreen 0
		if [ -z $WhatToFind ] ; then
			error "No parameter passed in calling function Check_Include"
		fi
		IdxToFind=0
		if [ -f "$Clip_S_Dir/$ARCH/$WhatToFind/.set" ] ; then
			CurIdx="$Clip_S_Dir/$ARCH/$WhatToFind/.set"
			lList="$(cat $CurIdx)"
			for fName in $lList ; do
				if [[ x"$fName" != x"" ]] ; then
					ArrayToFind[$IdxToFind]=$fName
					OnScreen 2 "Found : $WhatToFind requested by : $RequestedBy found in : $fName"
					echo $fName >"$Clip_S_Dir/$ARCH/$WhatToFind/.set"
					if [ -f $fName/$WhatToFind ] ; then
						echo $fName
						return
				  	fi
					let ++IdxToFind
				fi
			done
		fi
		if [ -n $WhereToFind ] ; then
			ArrayToFind[$IdxToFind]=$WhereToFind
			let ++IdxToFind
		fi
		ArrayToFind[$IdxToFind]=/usr/local
		let ++IdxToFind
		ArrayToFind[$IdxToFind]=/opt
		let ++IdxToFind
		if [ -d /include ] ; then
			ArrayToFind[$IdxToFind]=/include
			let ++IdxToFind
		fi
		ArrayToFind[$IdxToFind]=/usr/include
		let ++IdxToFind
		ArrayToFind[$IdxToFind]=/usr
		if [[ "$WaitForCheck" = [Yy] ]] ; then
			let ++IdxToFind
			ArrayToFind[$IdxToFind]=/
		fi
		if [[ $Extension != xyz ]] ; then
			WhatToFindTmp=$WhatToFind$Extension
		fi
		while [[ $varX = [nN] ]] ; do
			CurIdx=0
			while [ -z "$Found" ] && [ $CurIdx -le $IdxToFind ] ; do
				#OnScreen CurIdx : $CurIdx IdxToFind : $IdxToFind ArrayLen : ${#ArrayToFind[@]} contents : ${ArrayToFind[@]}
				#OnScreen CurIdx : $CurIdx IdxToFind : $IdxToFind Seach in : ${ArrayToFind[$CurIdx]}
				WhereToFind=${ArrayToFind[$CurIdx]}
				if [[ "$WhereToFind" = "/" ]] ; then
					banner "Not Found"
					OnScreen 1 "in : ${ArrayToFind[@]}"
					OnScreen 2
					tptptpt=0
					while [ $tptptpt -le $CommentNr ] ; do
						OnScreen 0 "${Comment[$tptptpt]}"
						let ++tptptpt
					done
					beep_on
					OnScreen 2 " Scan the all s[Y]stem : '$WhereToFind'"
					OnScreen 0 " or install package : $PackName"
					OnScreen 0 " and [R]escan standard directories"
					OnScreen 0 "${ArrayToFind[@]}"
					OnScreen 0 " or [N] skip : "
					OnScreen 2
					varX=" "
					read -n 1 -p "Your choice : [Y/N/R] : " varX >&0
					if [[ $varX = [nN] ]] ; then
						rm -f$V ../$RequestedBy.ok >&0
						touch ../$RequestedBy.no
						let ++CurIdx
						continue
					elif [[ $varX = [rR] ]] ; then
						CurIdx=0
						continue
					elif [[ $varX = [Yy] ]] ; then
						OnScreen 0
					else
						let --CurIdx
						continue
					fi
				fi
				#OnScreen 0 Working dir : $PWD
				OnScreen 0 "Searching for : $WhatToFind in : $WhereToFind requested by : $RequestedBy"
				if [ -f $WhereToFind/$WhatToFind ] ; then
					Found=$WhereToFind
				else
					Found=$(Check_Find $WhereToFind $WhatToFind )
				fi
				let ++CurIdx
			done
			if [ -z "$Found" ] ; then
				varX="Y"
				banner "Missing"																											>&0
				OnScreen 0 "Missing : $WhatToFind requested by : $RequestedBy 'Package name(s)' : $PackName"
				echo "Missing : $WhatToFind requested by : $RequestedBy Package name(s) : $PackName" 				>&2
				if [[ $Important = YES ]] ; then
					echo "#"																			>>$LogFile
					echo "This package is REQUIRED" 											>>$LogFile
					echo "#"																			>>$LogFile
				fi
				echo "Missing : $WhatToFind requested by : $RequestedBy" 			>>$LogFile
				for IdxToFind in $PackName ; do
					echo "Package name(s)" :  $IdxToFind 									>>$LogFile
				done
				rm -f$V ../$RequestedBy.ok	>&0
				touch ../$RequestedBy.no
			else
				beep_on
				OnScreen 2 "Found : $WhatToFind requested by : $RequestedBy found in : $Found"
				OnScreen 2 "[s]earch path; is it Ok [y/n/s] : "
				read -n 1 varX 	>&0
				OnScreen 2
				if [[ $varX = [yY] ]] ; then
					mkdir -p$V "$Clip_S_Dir/$ARCH/$WhatToFind/" >&0
					echo $Found >"$Clip_S_Dir/$ARCH/$WhatToFind/.set"
					# sleep $SleepTime
					# OnScreen 0
					if [[ "$Found/$WhatToFind" -nt Makefile.in ]] ; then
						touch Makefile.in
					fi
					echo $Found
					# sleep $SleepTime
				elif [[ $varX = [sS] ]] ; then
					OnScreen 1 "Search path : "
					read -p varX 	>&0
					ArrayToFind[0]=$varX
					Found=""
					varX="N"
				else
					rm -fv ../$RequestedBy.ok 	>&0
					touch ../$RequestedBy.no 	>&0
					Found=""
					echo "z" 	>&0
					varX="N"
				fi
			fi
		done
	}
# end function Check_Include

function Check_Library ()
	{
		#
		# verifies if the asked library exists
		#
		local LogFile="$Clip_T_Dir/asklibrary.log"
		local WhatToFind
		local Extension
		local RequestedBy
		local Important
		local WhereToFind
		local Found=""
		local PackName
		local ArrayToFind
		local IdxToFind
		local CurIdx
		unset ArrayToFind
		local Comment
		local CommentNr
		local varX="N"
		WhatToFind=lib$1
		shift
		Extension=$1
		shift
		RequestedBy=$1
		shift
		Important=$1
		Important=$(AllUpper $Important )
		#OnScreen 0 "Working dir :" $PWD
		if [[ $Important = YES ]] || [[ $Important = NO ]] ; then
			echo "" >/dev/null
		else
			error $*
		fi
		shift
		WhereToFind=$1
		shift
		PackName=""
		let CommentNr=-1
		IdxToFind=false
		while [ -n "$*" ] ; do
			if [ "$1" = "/" ] ; then
				IdxToFind=true
			elif  [ $IdxToFind = true ] ; then
				let ++CommentNr
				Comment[$CommentNr]=$1
			else
				PackName="$PackName $1"
			fi
			shift
		done
		OnScreen 0
		if [ -z $WhatToFind ] ; then
			error "No parameter passed in calling function Check_Library"
		fi
		IdxToFind=0
		if [ -f "$Clip_S_Dir/$ARCH/$WhatToFind/.set" ] ; then
			CurIdx="$Clip_S_Dir/$ARCH/$WhatToFind/.set"
			lList="$(cat $CurIdx)"
			for fName in $lList ; do
				if [[ x"$fName" != x"" ]] ; then
					OnScreen 2 "Found : $WhatToFind requested by : $RequestedBy found in : $fName"
					mkdir -p$V "$Clip_S_Dir/$ARCH/$WhatToFind/"
					echo $fName >"$Clip_S_Dir/$ARCH/$WhatToFind/.set"
					ArrayToFind[$IdxToFind]=$fName
					if [ -f $fName/$WhatToFind.so ] || [ -f $fName/$WhatToFind.a ] ; then
						OnScreen 2 "$fName $fName/$WhatToFind"
						echo $fName
						return
				  	fi
					let ++IdxToFind
				fi
			done
		fi
		if [ -n $WhereToFind ] ; then
			ArrayToFind[$IdxToFind]=$WhereToFind
			let ++IdxToFind
		fi
		ArrayToFind[$IdxToFind]=/usr/local
		let ++IdxToFind
		ArrayToFind[$IdxToFind]=/opt
		let ++IdxToFind
		ArrayToFind[$IdxToFind]=/$LibDir
		let ++IdxToFind
		ArrayToFind[$IdxToFind]=/usr/$LibDir
		if [[ "$WaitForCheck" = [Yy] ]] ; then
			let ++IdxToFind
			ArrayToFind[$IdxToFind]=/
		fi
		if [[ $Extension != xyz ]] ; then
			WhatToFindTmp=$WhatToFind$Extension
		fi
		while [[ $varX = [nN] ]] ; do
			CurIdx=0
			while [ -z "$Found" ] && [ $CurIdx -le $IdxToFind ] ; do
				#OnScreen CurIdx : $CurIdx IdxToFind : $IdxToFind ArrayLen : ${#ArrayToFind[@]} contents : ${ArrayToFind[@]}
				#OnScreen CurIdx : $CurIdx IdxToFind : $IdxToFind Seach in : ${ArrayToFind[$CurIdx]}
				WhereToFind=${ArrayToFind[$CurIdx]}
				if [[ "$WhereToFind" = "/" ]] ; then
					banner "Not Found"
					OnScreen 1 "in : ${ArrayToFind[@]}"
					OnScreen 2
					tptptpt=0
					while [ $tptptpt -le $CommentNr ] ; do
						OnScreen 0 "${Comment[$tptptpt]}"
						let ++tptptpt
					done
					beep_on
					OnScreen 2 " Scan the all s[Y]stem : '$WhereToFind'"
					OnScreen 0 " or install package : $PackName "
					OnScreen 0 " and [R]escan standard directories "
					OnScreen 0 "${ArrayToFind[@]}"
					OnScreen 0 " or [N] skip : "
					OnScreen 2
					varX=" "
					read -n 1 -p "Your choice : [Y/N/R] : " varX 	>&0
					if [[ $varX = [nN] ]] ; then
						rm -f$V ../$RequestedBy.ok 	>&0
						touch ../$RequestedBy.no
						let ++CurIdx
						continue
					elif [[ $varX = [rR] ]] ; then
						CurIdx=0
						continue
					elif [[ $varX = [Yy] ]] ; then
						OnScreen 0
					else
						let --CurIdx
						continue
					fi
				fi
				#OnScreen 0 Working dir : $PWD
				OnScreen 0 "Searching for : $WhatToFind$Extension in : $WhereToFind requested by : $RequestedBy"
				if [ -f $WhereToFind/$WhatToFind$Extension ] ; then
					Found=$WhereToFind
				else
					OnScreen 0 "Searching for : $WhatToFind$Extension in : $WhereToFind requested by : $RequestedBy"
					Found=$(Check_Find $WhereToFind $WhatToFind$Extension )
				fi
				if [ -z "$Found" ] ; then
						Extension=.so
					if [ -f $WhereToFind/$WhatToFind$Extension ] ; then
						Found=$WhereToFind
					else
						OnScreen 0 "Searching for : $WhatToFind$Extension in : $WhereToFind requested by : $RequestedBy"
						Found=$(Check_Find $WhereToFind $WhatToFind$Extension )
					fi
					if [ -z "$Found" ] ; then
						Extension=.a
						if [ -f $WhereToFind/$WhatToFind$Extension ] ; then
							Found=$WhereToFind
						else
							OnScreen 0 "Searching for : $WhatToFind$Extension in : $WhereToFind requested by : $RequestedBy"
							Found=$(Check_Find $WhereToFind $WhatToFind.so )
						fi
					fi
				fi
				let ++CurIdx
			done
			if [ -z "$Found" ] ; then
				varX="Y"
				banner "Missing" 																										>&0
				OnScreen 0 "Missing : $WhatToFind requested by : $RequestedBy Package name(s) : $PackName"
				echo "Missing : $WhatToFind requested by : $RequestedBy Package name(s) : $PackName" 			>&2
				if [[ $Important = YES ]] ; then
					echo "#"																			>>$LogFile
					echo "This package is REQUIRED" 											>>$LogFile
					echo "#"																			>>$LogFile
				fi
				echo "Missing : $WhatToFind requested by : $RequestedBy" 			>>$LogFile
				for IdxToFind in $PackName ; do
					echo "Package name(s) : $IdxToFind" 									>>$LogFile
				done
				rm -f$V ../$RequestedBy.ok 	>&0
				touch ../$RequestedBy.no
			else
				beep_on
				OnScreen 2 Found : $WhatToFind$Extension requested by : $RequestedBy found in : $Found
				OnScreen 2 "[s]earch path; is it Ok [y/n/s] : "
				read -n 1 varX 	>&0
				OnScreen 2
				if [[ $varX = [yY] ]] ; then
					mkdir -p$V "$Clip_S_Dir/$ARCH/$WhatToFind/" >&0
					echo $Found >"$Clip_S_Dir/$ARCH/$WhatToFind/.set"
					# sleep $SleepTime
					# OnScreen 0
					if [[ "$Found/$WhatToFind" -nt Makefile.in ]] ; then
						touch Makefile.in
					fi
					echo $Found
					# sleep $SleepTime
				elif [[ $varX = [sS] ]] ; then
					OnScreen 1 "Search path : "
					read -p varX 	>&0
					ArrayToFind[0]=$varX
					Found=""
					varX="N"
				else
					rm -f ../$RequestedBy.ok 	>&0
					touch ../$RequestedBy.no 	>&0
					Found=""
					echo "z" 	>&0
					varX="N"
				fi
			fi
		done
	}
# end function Check_Library

function Config_Init ()
	{
		rm -f$V ./Makefile ./Makefile.01 ./Makefile.ini ./configure.ini ./include || true
		source ../configure.ini
	   if [ $? != 0 ] ; then
			echo "Configuration for $PWD Config_Init"
			error $PWD
			exit 1
		fi
		cp --remove-destination -uf$V ../configure.ini ./
		cp --remove-destination -uf$V ../Makefile.ini ./
		[ -d include ] 	|| [ -L include ] 	|| ln -sf$V $Clip_I_Dir 			./include
		[ -d locale.po ] 	|| [ -L locale.po ] 	|| ln -sf$V $CLIPROOT/locale.po 	./locale.po
		[ -d charsets ] 	|| [ -L charsets ] 	|| ln -sf$V $CLIPROOT/charsets 	./charsets

	}
# end function Config_Init

function Config_Init_2 ()
	{
		source ./configure.ini
	   if [ $? != 0 ] ; then
			echo "Configuration for $PWD Config_Init_2"
			error $PWD
			exit 1
		fi

	}
# end function Config_Init_2

function CreateAllDirs ()
	{
		Clip_C_Dir="$Clip_M_Dir/config"
		Clip_S_Dir="$Clip_C_Dir/setup"
		Clip_T_Dir="$Clip_M_Dir/temp"
		Clip_D_Dir="$CLIPROOT/doc"
		Hash_Dir="$Clip_M_Dir/hasher/hashed"
		[ -d $Clip_S_Dir ] 	|| mkdir -p$V $Clip_S_Dir
		[ -d $Clip_T_Dir ] 	|| mkdir -p$V $Clip_T_Dir
		[ -d $Clip_B_Dir ] 	|| mkdir -p$V $Clip_B_Dir
		[ -d $Clip_D_Dir ] 	|| mkdir -p$V $Clip_D_Dir
		[ -d $Clip_I_Dir ] 	|| mkdir -p$V $Clip_I_Dir
		[ -d $Clip_L_Dir ] 	|| mkdir -p$V $Clip_L_Dir
		[ -d $Hash_Dir ] 		|| mkdir -p$V $Hash_Dir
	}
# end function CreateAllDirs ()

function CreateMakefile ()
	{
		MkExist=0
		if [ -f ./Makefile ] ; then
			MkExist=1
		fi
		sleep .1
		echo "# generated by configure" 					>Makefile
		cat $Makefile_01_in 									>>Makefile
		cat ./Makefile.ini 									>>Makefile
		cat ./configure.ini 									>/dev/null
	   if [ $? != 0 ] ; then
			echo "Configuration for $PWD"
			error $PWD
			exit 1
		fi
		#[ -f ./Makefile.01 ] &&
		cat ./Makefile.01 									>>Makefile
		cat $Makefile_gen_in 								>>Makefile
		cat ./Makefile.in 									>>Makefile
		if [[ -MkExist = 1  ]]; then
			error $PWD/Makefile $0 EXISTS!!!
		fi

	}

function create_var ()
	{
		#
		# export 1 variable by name
		#
		#
		# for ii in $xyz; do $ii='$($ii)'; done
		#
		# ii="aa"  >>>>> aa=xy
		#
		local yName
		yName=$1
		shift
		if [ -n "$*" ]
			then
			$yName="$*"
		fi
		export >/dev/null
	}
# end function create_var

function deception ()
	{
		if [ -x /usr/bin/mplayer ] ; then
			mplayer -vo null $Clip_M_Dir/sounds/k3b_error1.wav 1>/dev/null 2>/dev/null
			echo " pitty mplayer pitty" 	>&0
			echo "" 								>&0
		elif [ -x /usr/bin/play ] ; then
			play -ao alsa $Clip_M_Dir/sounds/k3b_error1.wav 1>/dev/null 2>/dev/null
			echo " pitty play pitty" 		>&0
			echo "" 								>&0
		else
			echo " pitty pitty" 				>&0
			echo "" 								>&0
		fi
	}
# end function deception

function error ()
	{
		#
		#  displays a message of given 'error message',
		#  waits for a key pressed
		# & breaks the program
		#
		if [ -x /usr/bin/mplayer ] ; then
			/usr/bin/mplayer -vo null $Clip_M_Dir/sounds/k3b_error1.wav 1>/dev/null 2>/dev/null
			echo " pitty mplayer pitty" 	>&0
			echo "" 								>&0
		elif [ -x /usr/bin/play ] ; then
			/usr/bin/play -ao alsa $Clip_M_Dir/sounds/k3b_error1.wav 1>/dev/null 2>/dev/null
			echo " pitty play pitty" 		>&0
			echo "" 								>&0
		else
			beep_on
			beep_on
			beep_on
			echo " pitty pitty" 				>&0
			echo "" 								>&0
		fi
		OnScreen 1 "...............error.............."
		OnScreen 1 "...............error.............."
		OnScreen 0
		while [ -n "$*" ] ; do
			OnScreen 0 error : $1
			shift
		done
		deception
		warning ".........error.........breaking !!!!"
		read -p "Press <enter> to continue"
	}
# end function error

function ETAtime ()
	{
		#
		# calculates and returns E.T.A
		#
		local Etime
		local ElapsedTime
		local TotalRemaining
		local ToEcho
		local Total
		local TotalDone
		local TTtimeDone
		local TTtimeEtime
		TotalDone=$1
		Total=$2
		if [ $TotalDone -le 0 ] ; then
			let TotalDone=1
		fi
		let TotalRemaining=$Total-$TotalDone
		let ElapsedTime=$(date +%s)
		let ElapsedTime=$ElapsedTime-$StartTime
		let Etime=($TotalRemaining*$ElapsedTime)
		let Etime=$Etime/$TotalDone
		let TTtimeDone=$ElapsedTime*2
		let TTtimeEtime=$Etime*$lEtime
		#&& [[ $lTTtimeEtime -lt $lTTtimeDone ]]
		if [ $StartTotal -eq 0 ] ; then
			let Etime=$Etime*$Etime
			let ElapsedTime=$ElapsedTime*2
			let Etime=$lEtime+$ElapsedTime
			let Etime=$Etime/2
		fi
		ToEcho=$(SecToTime $Etime )
		echo ETA : $ToEcho
	}
# end ETAtime


function Execute_Command ()
	{
		#
		#  executes a command given by parameter
		#
		#  e.g. : to touch a lock file  $Lock 'constant' $Var
		# $Lockconstant$Var will not work
		# as parameters xx=$1$2$3
		# $xx
		#
		#
		# only alone character '|' (pipe) delimits variables (space value)
		#
		# $1=$xyz
		# $2='temp'
		# $3=$ii
		# $4=$jj
		# $5='='
		# $6=$kk
		# $7='|'
		# $8=$ll
		#
		#  >>>>>>>>>>> ToExecute='$1$2$3$4$5"$6 $8"'
		# $ToExecute
		local Parameter="$1"
		local xy
		local ToExecute=""
		local ArrayX
		local ii
		echo "Execute_Command : $*"  >>tt.sh
		let xy=-1
		while ! [ -z "$Parameter" ] ; do
			let ++xy
			if [ "$Parameter" == "|" ] ; then
				ArrayX[$xy]=' '
			else
				ArrayX[$xy]=$Parameter
			fi
			shift
		done
		while [ $xy -ne -1 ] ; do
			ii="${ArrayX[$xy]}$ToExecute"
			ToExecute="$ii"
			let --xy
		done
		echo $ToExecute >>tt.sh
		$ToExecute
	}
# end function Execute_Command

function GetArch ()
	{
		local yz
		local xy
		local ARCH=""
		local varX
		local C64
		local trt
		local nii
		xy=0
		let yz=1
		if [[ "1" = "$*" ]] ; then
			let varX=-1
		else
			varX="$*"
			[ -z "$varX" ] && varX=0
		fi
		if [ -f $Clip_S_Dir/ARCH.setup.ini ] ; then
			ARCH=$(cat "$Clip_S_Dir/ARCH.setup.ini")
		fi
		for nii in $ARCH ; do
			ARCH=$nii
		done
		C64=$(uname -m)
		if [ -n "$ARCH" ] ; then
			if [[ $ARCH = "x86_64" ]] && [[ $C64 = "x86_64" ]] ; then
				let yz=2
			else
				let yz=1
			fi
		else
			let yz=-1
		fi
		if [ $varX -eq -1 ] ; then
			let yz=-1
		fi
		while [ $yz -eq -1 ] ; do
			rm -fv$V $Clip_S_Dir/* 			>&0
			rm -Rf$V $INSTDIR 	>&0
			varX=0
			OnScreen 10 "You may install :"
			OnScreen 1 "[ 1 ] : i386/i586 version "
			if [[ "$C64" = "x86_64" ]] ; then
				OnScreen 1 "if you want to compile for 32 bits, you NEED TO INSTALL 32 bits libraries"
				OnScreen 1 "[ 2 ] : x86_64 version "
				OnScreen 0
			fi
			read -n 1 -p "			Your choice : " varX >&0
			trt=$varX
			if [ $trt -le 0 ] ; then
				let trt=-1
			elif [ $trt -gt 2 ] ; then
				beep_on
				let trt=-1
			elif [ $trt -eq 2 ] && [[ $C64 = "x86_64" ]] ; then
				beep_on
			fi
			yz=$trt
			if [ $yz -eq 2 ] && [[ $C64 = "x86_64" ]] ; then
				ARCH=x86_64
			else
				ARCH=i586
			fi
			[ -d $Clip_S_Dir ] || mkdir -p$V $Clip_S_Dir
			echo $ARCH >$Clip_S_Dir/ARCH.setup.ini
		done
		if [ $yz -eq 2 ] && [[ $C64 = "x86_64" ]] ; then
			ARCH=x86_64
		else
			ARCH=i586
		fi
		echo $ARCH
	}
# end function GetArch

function libcver ()
{
	local GLIBC
	local GLIBC_MINOR
	local GNU_LIBRARY
	GLIBC=$(grep '^#define[ 	]*__GLIBC__' /usr/include/features.h 2>/dev/null | tr -s ' 	' ' ' | cut -d' ' -f3)
	GLIBC_MINOR=$(grep '^#define[ 	]*__GLIBC_MINOR__' /usr/include/features.h 2>/dev/null | tr -s ' 	' ' ' | cut -d' ' -f3)
	GNU_LIBRARY=$(grep '^#define[ 	]*__GNU_LIBRARY__' /usr/include/features.h 2>/dev/null | tr -s ' 	' ' ' | cut -d' ' -f3)
	#echo $GLIBC $GLIBC_MINOR $GNU_LIBRARY
	if [ -n "$GLIBC" -a -n "$GLIBC_MINOR" ] ; then
		echo "glibc$GLIBC.$GLIBC_MINOR"
	elif [ -n "$GNU_LIBRARY" ] ; then
		echo "libc$GNU_LIBRARY"
	else
		name="$(uname -r|cut -d '(' -f 1)"
		if [ -n "$name" ] ; 	then
			echo "$name"
		else
			echo "libc_unknown"
		fi
	fi
}
# end function libcver

function MakeDocDir ()
	{
		if [ -n $* ] ; then
			# echo "Clip_D_Dir=$Clip_D_Dir"
			[ -d $Clip_D_Dir/$1 ] 			|| mkdir -p$V $Clip_D_Dir/$1
			[ -d $Clip_D_Dir/example/$1 ] || mkdir -p$V $Clip_D_Dir/example/$1
		fi
	}
function oneof ()
	{
		#
		#  returns the 1st existing file
		#
		for i in $* ; do
			if [ -f $i ] ; then
				echo $i
				return 0
			fi
		done
	}
# end function oneof


function oneofdir ()
	{
		#
		#  returns the 1st existing dir
		#
		for i in $* ; do
			if [ -d $i ] ; then
				echo $i
				return 0
			fi
		done
		return 0
	}
# end function oneofdir

function OnAllScreen ()
{
	echo $* 	>&0
	echo $* 	>&1
	echo $* 	>&2

}
#end function OnAllScreen ()

function OnScreen ()
	{
		##
		## forces display on tty if redirection used  '1>file'
		##
		## displays a message on the screen even if the outputs are redirected ( 1>xx 2>xx )
		##
		local xx
		local yy="$1"
		if ( [[ "$yy" < "A" ]] && [[ $yy -ge 0 ]] ) ; then
			xx=$yy
			shift
		else
			xx=0
		fi
		while [ $xx -gt 1 ] ; do
			echo "" >&0
			let --xx
		done
		echo $* >&0
	}
# end function OnScreen

function osname ()
	{
		local uname
		local osname
		local rname
		uname=`uname -s 2>/dev/null`
		osname=$uname
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
				rname=`uname -r 2>/dev/null`
				case "$rname" in
					"5.7")
						osname=SOLARIS_7
   	            ;;
					"5.8")
						osname=SOLARIS_8
						;;
					"5.9")
						osname=SOLARIS_9
						;;
				esac
				;;
			CYGWIN*)
				osname=CYGWIN
				;;
		esac
	echo $osname
}
function readans ()
	{
		#
		# ????????????????????????
		#
		local ans
		if [ -z "$yesf" ] ; then
			printf  "$1 [$2]: " >&2
			read ans <&2
		else
			echo "$1 [$2]:" >&2
		fi
		if [ -n "$ans" ] ; then
			echo $ans
		else
			echo $2
		fi
	}
# end function readans

function ReadVarX_Min_Max ()
	{
		#
		#  displays a message for getting a value within a range  0 to n,
		#  forces selection
		#  & sets variable according to selection.
		#
		#  ReadVarX_Min_Max  Message	$1
		# MinValue	$3
		# MaxValue	$4
		# Value 1	$5
		# Value 2	$6
		# Value n	$(1+n)
		# OnScreen $*
		#  returns selected value
		local Values
		local xx
		local Message=$1
		shift
		local MinValue=$1
		shift
		local MaxValue=$1
		shift
		if [[ $MinValue != [0Nn] ]] ; then
			error ReadVarX_Min_Max $*
		fi
		#if [[ $MaxValue != [[0-9][YyNn]] ]] ; then
		#	error ReadVarX_Min_Max $*
		#fi
		xy=0
		while [ -n "$*" ] ; do
			Values[$xy]=$1
			let ++xy
			shift
		done
		trt=-1
		while [ $trt -gt $MaxValue ] || [ $trt -lt $MinValue ] ; do
			OnScreen 0 $Message
			read -n 1 -p " Values : [ $MinValue >> $MaxValue ] : " trt>&0
		done
		if [[ ${Values[$trt]} = "/" ]] ; then
			echo ""
		else
			echo ${Values[$trt]}
		fi
	}
# end function ReadVarX_Min_Max

function SecToTime ()
	{
		local lDays
		local lHours
		local lMinutes
		local lSeconds
		local lToEcho
		#echo $* >&0
		lDays=0
		lHours=0
		lMinutes=0
		lSeconds=$1
		lToEcho=
		while [ $lSeconds -ge 86400 ] ; do
			let ++lDays
			let lSeconds=$lSeconds-86400
		done
		if [ $lDays -gt 0 ] ; then
			lToEcho="d$lDays"
		fi
		while [ $lSeconds -ge 3600 ] ; do
			let ++lHours
			let lSeconds=$lSeconds-3600
		done
		if [ $lHours -gt 9 ] && [ -n $lToEcho ] ; then
			lToEcho="$lToEcho.$lHours"
		elif [ $lHours -gt 0 ] && [ -n $lToEcho ] ; then
			lToEcho="$lToEcho.0$lHours"
		elif [ -n $lToEcho ] ; then
			lToEcho="$lToEcho.00"
		elif [ $lHours -gt 0 ] ; then
			lToEcho="$lHours"
		fi
		while [ $lSeconds -ge 60 ] ; do
			let ++lMinutes
			let lSeconds=$lSeconds-60
		done
		if [ $lMinutes -gt 9 ] && [ -n $lToEcho ] ; then
			lToEcho="$lToEcho:$lMinutes"
		elif [ $lMinutes -gt 0 ] && [ -n $lToEcho ] ; then
			lToEcho="$lToEcho:0$lMinutes"
		elif [ -n $lToEcho ] ; then
			lToEcho="$lToEcho:00"
		elif [ $lMinutes -gt 0 ] ; then
			lToEcho="$lMinutes"
		fi
		if [ $lSeconds -gt 9 ] && [ -n $lToEcho ] ; then
			lToEcho="$lToEcho:$lSeconds"
		elif [ $lSeconds -gt 0 ] && [ -n $lToEcho ] ; then
			lToEcho="$lToEcho:0$lSeconds"
		elif [ -n $lToEcho ] ; then
			lToEcho="$lToEcho:00"
		else
			lToEcho="$lSeconds s"
		fi
		echo $lToEcho
	}
# end function SecToTime

function StatusScreen ()
	{
		echo $* >&0
	}
# end function StatusScreen

function temp_file ()
	{
		Temp_File_Name=$Clip_T_Dir/temp.$Temp_File_Number
		let ++Temp_File_Number
		echo $Temp_File_Name
	}
# end function temp_file

function TotalFiles ()
	{
		local njj
		local ljj
		local lList
		njj=0
		echo Calculating ... >&0
		ljj=$$
		ls -AR >"$HOME/tmp$ljj"
		lList=$(cat "$HOME/tmp$ljj")
		rm -f ./tmp$ljj >/dev/null
		for ljj in $lList ; do
			let ++njj
 		done
 		lList=
 		echo $njj
	}
# end TotalFiles

function trumpet ()
	{
		if [ -x /usr/bin/mplayer ] ; then
			/usr/bin/mplayer -vo null $Clip_M_Dir/sounds/k3b_success1.wav 1>/dev/null 2>/dev/null
			echo " successful mplayer" 	>&0
			echo "" 								>&0
		elif [ -x /usr/bin/play ] ; then
			/usr/bin/play -ao alsa $Clip_M_Dir/sounds/k3b_success1.wav 1>/dev/null 2>/dev/null
			echo " successful play " 		>&0
			echo "" 								>&0
		else
			beep_on
			echo " successful" 				>&0
			echo "" 								>&0
		fi
	}
# end function trumpet

function warning ()
	{
		#
		#  displays a message of given 'warning message',
		#  waits for a key pressed
		# & goes on.
		#
		OnScreen 1 "WARNING !!!"
		OnScreen 0
		while [ -n "$*" ] ; do
			OnScreen 0 $1
			shift
		done

		beep_on
		OnScreen 1 ".................press any key"
		OnScreen 0
		read -n 1 y >&0
		OnScreen 0
	}

function xoneof ()
	{
		#
		#  returns the 1st executable file
		#
		for i in $* ; do
			if [ -x $i ] ; then
				echo $i
				return 0
			fi
		done
		return 0
	}

##
############################################################################################################
############################################################################################################
##                                                                                                        ##
##                                                                                                        ##
##            END  OF functions DECLARATION                                                               ##
##                                                                                                        ##
##                                                                                                        ##
##                                                                                                        ##
############################################################################################################
############################################################################################################
