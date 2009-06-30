#!/bin/bash -ue
#
# initiates or updates Makefile parameters & behaviour
#
source $Clip_F_F
export List=""
	if [ -f $Clip_S_Dir/Makefile.setup.ini ] ; then
		export List=$(cat $Clip_S_Dir/Makefile.setup.ini )
	else
		export List="
			Debug_Make
			E_c_cp
			E_c_mkl
			E_c_mkSl
			E_cc
			E_cd
			E_clip
			E_cp
			E_e
			E_for
			E_if
			E_ln
			E_mkD
			E_rm
			E_xx
		"
	fi
	trt="y"
	aList=""
	xy=0
	for nii in $List ; do
		xx=""
	   if [ -f $Clip_S_Dir/$nii.setup.ini ] ; then
			xx="$(cat $Clip_S_Dir/$nii.setup.ini)"
			export $nii="$xx"
		else
			export $nii=""
			trt=n
		fi
		aList[$xy]=$nii
		bList[$xy]="$xx"
		let ++xy
	done
#	OnScreen $xy
#	OnScreen ${#aList[@]}
#	OnScreen ${aList[@]}
	while [[ "$trt" = n ]] ; do
		OnScreen 5 'Configuration of clip, tools & libraries'
		OnScreen 0 'Some parameters need to be given to suit your needs'
		OnScreen 0
		OnScreen 0 "Your personal settings :"
		OnScreen 0 "				the next version will be more performent"
		trt=-1
		OnScreen 0
		OnScreen 0 "for all Makefile command(s) for debugging & tracking 'Makefile'"
		OnScreen 0 "target(s) : prerequisite(s)"
		OnScreen 0 ' ' "@echo xvxv  (line command is NOT echoed)"
		OnScreen 0 ' ' "cp jklm	(line command is echoed)"
		OnScreen 0 ' ' "cp -f\$V jklm	(command is verbose)"
		xy=0
		OnScreen 0
		while [ $xy -lt ${#aList[@]} ] ; do
#			OnScreen $xy
			OnScreen 3 Variable: ${aList[$xy]}
			if [[ ${aList[$xy]} = E_c_cp ]] ; then
				export E_c_cp=$(ReadVarX_Min_Max 		"Echo command 'clip_cp ' 			suggested (1=trace/debug) 	in Makefile(s) : " 	0 1 	"@" "/" )
				bList[$xy]=$E_c_cp
			elif [[ ${aList[$xy]} = E_c_mkl ]] ; then
				export E_c_mkl=$(ReadVarX_Min_Max 	"Echo command 'clip_makelib ' 	suggested (1=trace/debug) 	in Makefile(s) : " 	0 1 	"@" "/" )
				bList[$xy]=$E_c_mkl
			elif [[ ${aList[$xy]} = E_c_mkSl ]] ; then
				export E_c_mkSl=$(ReadVarX_Min_Max 	"Echo command 'clip_makesklib ' 	suggested (1=trace/debug) 	in Makefile(s) : " 	0 1 	"@" "/" )
				bList[$xy]=$E_c_mkSl
			elif [[ ${aList[$xy]} = E_cc ]] ; then
				export E_cc=$(ReadVarX_Min_Max 		"Echo command 'cc ' 					suggested (1=trace/debug) 	in Makefile(s) : " 	0 1 	"@" "/" )
				bList[$xy]=$E_cc
			elif [[ ${aList[$xy]} = E_cd ]] ; then
				export E_cd=$(ReadVarX_Min_Max 		"Echo command 'cd ' 					suggested (1=trace/debug) 	in Makefile(s) : " 	0 1 	"@" "/" )
				bList[$xy]=$E_cd
			elif [[ ${aList[$xy]} = E_clip ]] ; then
				export E_clip=$(ReadVarX_Min_Max 		"Echo command 'clip ' 				suggested (1=trace/debug) 	in Makefile(s) : " 	0 1 	"@" "/" )
				bList[$xy]=$E_clip
			elif [[ ${aList[$xy]} = E_cp ]] ; then
				export E_cp=$(ReadVarX_Min_Max 		"Echo command 'cp ' 					suggested (1=trace/debug) 	in Makefile(s) : " 	0 1 	"@" "/" )
				bList[$xy]=$E_cp
			elif [[ ${aList[$xy]} = E_e ]] ; then
				export E_e=$(ReadVarX_Min_Max 			"Echo command 'echo' 				suggested (1=trace/debug) 	in Makefile(s) : " 	0 1 	"@" "/" )
				bList[$xy]=$E_e
			elif [[ ${aList[$xy]} = E_for ]] ; then
				export E_for=$(ReadVarX_Min_Max 		"Echo command 'for' 					suggested (1=trace/debug) 	in Makefile(s) : " 	0 1 	"@" "/" )
				bList[$xy]=$E_for
			elif [[ ${aList[$xy]} = E_if ]] ; then
				export E_if=$(ReadVarX_Min_Max 		"Echo command 'if ' 					suggested (1=trace/debug) 	in Makefile(s) : " 	0 1 	"@" "/" )
				bList[$xy]=$E_if
			elif [[ ${aList[$xy]} = E_ln ]] ; then
				export E_ln=$(ReadVarX_Min_Max 		"Echo command 'ln ' 					suggested (1=trace/debug) 	in Makefile(s) : " 	0 1 	"@" "/" )
				bList[$xy]=$E_ln
			elif [[ ${aList[$xy]} = E_mkD ]] ; then
				export E_mkD=$(ReadVarX_Min_Max 		"Echo command 'mkdir ' 				suggested (1=trace/debug) 	in Makefile(s) : " 	0 1 	"@" "/" )
				bList[$xy]=$E_mkD
			elif [[ ${aList[$xy]} = E_rm ]] ; then
				export E_rm=$(ReadVarX_Min_Max 		"Echo command 'rm ' 					suggested (1=trace/debug) 	in Makefile(s) : " 	0 1 	"@" "/" )
				bList[$xy]=$E_rm
			elif [[ ${aList[$xy]} = E_xx ]] ; then
				export E_xx=$(ReadVarX_Min_Max 		"Echo command 'xx ' 					suggested (1=trace/debug) 	in Makefile(s) : " 	0 1 	"@" "/" )
				bList[$xy]=$E_xx
			elif [[ ${aList[$xy]} = Debug_Make ]] ; then
				export Debug_Make=$(ReadVarX_Min_Max 		"Echo some debugging traces in Makefile 	suggested (1=trace/debug) 	in Makefile(s) : " 	0 1 	0 1 )
				bList[$xy]=$Debug_Make
			elif [[ ${aList[$xy]} = MAKEFLAGS ]] ; then
				xyz=" "
				OnScreen 1 Variable: ${aList[$xy]}
				read -p "New value : : " xyz
				bList[$xy]=$xyz
			elif [[ -n ${aList[$xy]} ]] ; then
				xyz=" "
				OnScreen 1 Variable: ${aList[$xy]}
				read -p "New value : (empty to delete) : " xyz 					>&0
				bList[$xy]="$xyz"
			fi
			let ++xy
		done
#		OnScreen $xy
		xyz="y"
		while [[ $xyz = "y" ]] ; do
			OnScreen 1
			read -n 1 -p "Add new variable(s) y/n : " trt 						>&0
			if [[ "$trt" = [Yy] ]] || [[ "$trt" = Y ]] || [[ "$trt" = y ]] ; then
				read -p "New variable name : (empty to skip) : " xyz 			>&0
				if [ -n $xyz ] ; then
					aList[$xy]="$xyz"
					OnScreen 1 New Variable: ${aList[$xy]}
					read -p "New value : (empty to delete) : " xyz 				>&0
					if [ -n $xyz ] ; then
						bList[$xy]=$xyz
						let ++xy
						xyz="y"
					else
						aList[$xy]=""
						xyz="n"
					fi
				else
					aList[$xy]=""
					xyz="n"
				fi
			else
				xyz=$trt
			fi
		done
#		OnScreen $xy
		OnScreen 13 "Value of set variables for debugging & tracing 'Makefile'"
		xy=0
		OnScreen 0
		while [ $xy -lt ${#aList[@]} ] ; do
			OnScreen 1 Variable: ${aList[$xy]} set to ${bList[$xy]}
			let ++xy
		done
		OnScreen 5
		read -n 1 -p "Is that what you need? [ Y/N ] : " trt 						>&0
		if [[ "$trt" = [Yy] ]] ; then
			OnScreen 0
		elif [[ "$trt" = Y ]] ; then
			OnScreen 0
		elif [[ "$trt" = y ]] ; then
			OnScreen 0
		elif [[ "$trt" = [Nn] ]] ; then
			OnScreen 0
			trt=n
		elif [[ "$trt" = N ]] ; then
			OnScreen 0
			trt=n
		elif [[ "$trt" = n ]] ; then
			OnScreen 0
			trt=n
		else
			trt=n
		fi
	done
	[ -d $Clip_S_Dir ] || mkdir -p$V $Clip_S_Dir
	xy=0
#	OnScreen 0
	while [ $xy -lt ${#aList[@]} ] ; do
		xyz=${aList[$xy]}
		echo ${bList[$xy]}	>$Clip_S_Dir/$xyz.setup.ini
		let ++xy
	done
	echo ${aList[@]} >$Clip_S_Dir/Makefile.setup.ini
	echo ${aList[@]}
