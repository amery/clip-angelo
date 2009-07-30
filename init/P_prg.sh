#!/bin/bash -ue
#
# initiates or updates CC & clip compilers parameters for prg dir
#
source $Clip_F_F
export C_PRG_FLAGS_ADDITIONNAL=" "
export CLIP_PRG_FLAGS_ADDITIONNAL=" "
export List=""
[ -f $Clip_S_Dir/prg.setup.ini ] && export List=$(cat $Clip_S_Dir/prg.setup.ini )

[ -z $List ] && export List="
	CFLAGS_GEN
	C_PRG_FLAGS
	CLIP_FLAGS_GEN
	CLIP_PRG_FLAGS
	CLIP_FLAGS_GEN_SHORT
	CLIP_PRG_SHORT_FLAGS
	PrgVerbose
"
#"	C_DEBUG_PRG_FLAGS
#	CLIP_DEBUG_PRG_FLAGS
#	"
export PrgVerbose="v"
for nii in $List ; do
	if [ -f $Clip_S_Dir/$nii.setup.ini ] ; then
		xx="$(cat $Clip_S_Dir/$nii.setup.ini)"
		export $nii="$xx"
	else
		PrgVerbose="-"
	fi
done

if [ $PrgVerbose == "-" ] ; then
	trt=n
	while [ "$trt" = n ] ; do
		OnScreen 10 'Configuration of clip, tools & libraries'
		OnScreen 0 'Some parameters need to be given to suit your needs'
		OnScreen 2 "While compiling 'PRG' subdirectories"
		OnScreen 2
		export PrgVerbose=$(ReadVarX_Min_Max 							" Desable '-v' in command(s) 	(suggested 0) : " 	0 1 	"v" "/" )
#		OnScreen 2 "For NON DEBUG version mode : 'Production'"
		OnScreen 3 " For CC compiling :"
		OnScreen 2 "\$ CC $C_PRG_FLAGS source.c"
		export C_PRG_FLAGS_VERBOSITY=$(ReadVarX_Min_Max 				" CC verbosity '--verbose' 	(suggested 0) : " 	0 1 	"/" '--verbose' )
		OnScreen 0
		export C_PRG_FLAGS_DEBUG_LEVEL=$(ReadVarX_Min_Max 			" CC debug '-gN' 					(suggested 0) : " 	0 3 	0 1 2 3 )
		OnScreen 0
		export C_PRG_FLAGS_OPTIMISE_LEVEL=$(ReadVarX_Min_Max 		" CC optimise -On 				(suggested 0) : " 	0 3 	0 1 2 3 )
		OnScreen 0
		export C_PRG_FLAGS_ADDITIONNAL=" "
		OnScreen 2
		read -p "Any additionnal parameter(s) : " C_PRG_FLAGS_ADDITIONNAL >&0
		OnScreen 3 " For PRG compiling :"
		OnScreen 2 "\$ clip $CLIP_PRG_FLAGS source.prg"
		OnScreen 0
		export CLIP_PRG_FLAGS_VERBOSITY=$(ReadVarX_Min_Max 			" clip verbosity 					(suggested 0) : " 	0 6 	0 1 2 3 4 5 6 )
		OnScreen 0
		export CLIP_PRG_FLAGS_DEBUG_LEVEL=$(ReadVarX_Min_Max 		" clip debug 						(suggested 0) : " 	0 1 	0 1 )
		OnScreen 0
		export CLIP_PRG_FLAGS_OPTIMISE_LEVEL=$(ReadVarX_Min_Max 	" clip optimise 					(suggested 0) : " 	0 3 	0 1 2 3 )
		OnScreen 0
		export CLIP_PRG_FLAGS_ADDITIONNAL=" "
		OnScreen 2
		read -p "Any additionnal parameter(s) : " CLIP_PRG_FLAGS_ADDITIONNAL >&0
		OnScreen 3
#		OnScreen 0 "For DEBUG || TEST version mode :"
#		OnScreen 0
#		OnScreen 0 " For CC compiling :"
#		OnScreen 0
#		C_DEBUG_PRG_FLAGS_VERBOSITY=$(ReadVarX_Min_Max 		" CC verbosity 					(suggested 1) : " 	0 1 	"/" "--verbose" )
#		C_DEBUG_PRG_FLAGS_DEBUG_LEVEL=3
#		C_DEBUG_PRG_FLAGS_OPTIMISE_LEVEL=0
#		OnScreen 2
#		OnScreen 0 " For Clip compiling :"
#		OnScreen 0
#		CLIP_DEBUG_PRG_FLAGS_VERBOSITY=$(ReadVarX_Min_Max 	" clip verbosity 					(suggested 2) : " 	0 6 	0 1 2 3 4 5 6 )
#		CLIP_DEBUG_PRG_FLAGS_DEBUG_LEVEL=1
#		CLIP_DEBUG_PRG_FLAGS_OPTIMISE_LEVEL=0
		# CC compiler's flags
		export C_PRG_FLAGS="$CFLAGS_GEN $C_PRG_FLAGS_VERBOSITY"
		export C_PRG_FLAGS="$C_PRG_FLAGS -O$C_PRG_FLAGS_OPTIMISE_LEVEL"
		export C_PRG_FLAGS="$C_PRG_FLAGS -g$C_PRG_FLAGS_DEBUG_LEVEL "
		export C_PRG_FLAGS="$C_PRG_FLAGS $C_PRG_FLAGS_ADDITIONNAL"
		# clip compiler's flags
		export CLIP_PRG_FLAGS="$CLIP_FLAGS_GEN --optimise=$CLIP_PRG_FLAGS_OPTIMISE_LEVEL"
		export CLIP_PRG_FLAGS="$CLIP_PRG_FLAGS --verbose=$CLIP_PRG_FLAGS_VERBOSITY"
		export CLIP_PRG_FLAGS="$CLIP_PRG_FLAGS --noremove-c "
		export CLIP_PRG_FLAGS="$CLIP_PRG_FLAGS --debug=$CLIP_PRG_FLAGS_DEBUG_LEVEL"
		export CLIP_PRG_FLAGS="$CLIP_PRG_FLAGS $CLIP_PRG_FLAGS_ADDITIONNAL"
		##  for test purposes
		# CC compiler's flags
#		C_DEBUG_PRG_FLAGS="$CFLAGS_GEN_DEBUG $C_DEBUG_PRG_FLAGS_VERBOSITY"
#		C_DEBUG_PRG_FLAGS="$C_DEBUG_PRG_FLAGS -O$C_DEBUG_PRG_FLAGS_OPTIMISE_LEVEL"
#		C_DEBUG_PRG_FLAGS="$C_DEBUG_PRG_FLAGS -g$C_DEBUG_PRG_FLAGS_DEBUG_LEVEL "
		# clip compiler's flags
#		CLIP_DEBUG_PRG_FLAGS="$CLIP_FLAGS_GEN_DEBUG --optimise=$CLIP_DEBUG_PRG_FLAGS_OPTIMISE_LEVEL"
#		CLIP_DEBUG_PRG_FLAGS="$CLIP_DEBUG_PRG_FLAGS --verbose=$CLIP_DEBUG_PRG_FLAGS_VERBOSITY"
#		CLIP_DEBUG_PRG_FLAGS="$CLIP_DEBUG_PRG_FLAGS --noremove-c"
#		CLIP_DEBUG_PRG_FLAGS="$CLIP_DEBUG_PRG_FLAGS --debug=$CLIP_DEBUG_PRG_FLAGS_DEBUG_LEVEL"
		OnScreen 10 "Value of set variables for compiling 'prg' subdirectories"
		OnScreen 2 "For cmd e.g : rm -f$PrgVerbose"
#		OnScreen 2 "For NON DEBUG version mode : 'Production'"
		OnScreen 1 "For CC compiling :"
		OnScreen 1 "  cc command line will be like :"
		OnScreen 1 "\$ CC $C_PRG_FLAGS source.c"
		OnScreen 1 " For Clip compiling :"
		OnScreen 1 "  clip command line will be like :"
		OnScreen 1 "\$ clip $CLIP_PRG_FLAGS source.prg"
#		OnScreen 2 "For DEBUG || TEST version mode :"
#		OnScreen 1 " For CC compiling :"
#		OnScreen 1 "  cc command line will be like :"
#		OnScreen 1 "\$ $CC $C_DEBUG_PRG_FLAGS source.c"
#		OnScreen 1 " For Clip compiling :"
#		OnScreen 1 "  clip command line will be like :"
#		OnScreen 1 "\$ clip $CLIP_DEBUG_PRG_FLAGS source.prg"
		OnScreen 5
		read -n 1 -p "Is that what you need? [ Y/N ] : " trt 					>&0
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
fi
# CC compiler's flags
if ! [[ $C_PRG_FLAGS_VERBOSITY == --verbose ]] ; then
	export C_PRG_FLAGS_VERBOSITY=""
fi
#if ! [[ $C_DEBUG_PRG_FLAGS_VERBOSITY == --verbose ]] ; then
#	C_DEBUG_PRG_FLAGS_VERBOSITY=""
#fi
export C_PRG_FLAGS="$CFLAGS_GEN $C_PRG_FLAGS_VERBOSITY"
export C_PRG_FLAGS="$C_PRG_FLAGS -O$C_PRG_FLAGS_OPTIMISE_LEVEL -g$C_PRG_FLAGS_DEBUG_LEVEL"
export C_PRG_FLAGS="$C_PRG_FLAGS $C_PRG_FLAGS_ADDITIONNAL"
# clip compiler's flags
export CLIP_PRG_FLAGS="$CLIP_FLAGS_GEN --optimise=$CLIP_PRG_FLAGS_OPTIMISE_LEVEL"
export CLIP_PRG_FLAGS="$CLIP_PRG_FLAGS --verbose=$CLIP_PRG_FLAGS_VERBOSITY"
export CLIP_PRG_FLAGS="$CLIP_PRG_FLAGS --debug=$CLIP_PRG_FLAGS_DEBUG_LEVEL"
export CLIP_PRG_FLAGS="$CLIP_PRG_FLAGS $CLIP_PRG_FLAGS_ADDITIONNAL"
export CLIP_PRG_SHORT_FLAGS="$CLIP_FLAGS_GEN_SHORT --optimise=$CLIP_PRG_FLAGS_OPTIMISE_LEVEL"
export CLIP_PRG_SHORT_FLAGS="$CLIP_PRG_SHORT_FLAGS -v$CLIP_PRG_FLAGS_VERBOSITY"
export CLIP_PRG_SHORT_FLAGS="$CLIP_PRG_SHORT_FLAGS --debug=$CLIP_PRG_FLAGS_DEBUG_LEVEL"
export CLIP_PRG_SHORT_FLAGS="$CLIP_PRG_SHORT_FLAGS $CLIP_PRG_FLAGS_ADDITIONNAL"
##  for test purposes
## CC compiler's flags
#C_DEBUG_PRG_FLAGS="$CFLAGS_GEN_DEBUG $C_DEBUG_PRG_FLAGS_VERBOSITY"
#C_DEBUG_PRG_FLAGS="$C_DEBUG_PRG_FLAGS -O$C_DEBUG_PRG_FLAGS_OPTIMISE_LEVEL"
#C_DEBUG_PRG_FLAGS="$C_DEBUG_PRG_FLAGS -g$C_DEBUG_PRG_FLAGS_DEBUG_LEVEL"
# clip compiler's flags
#CLIP_DEBUG_PRG_FLAGS="$CLIP_FLAGS_GEN_DEBUG --optimise=$CLIP_DEBUG_PRG_FLAGS_OPTIMISE_LEVEL"
#CLIP_DEBUG_PRG_FLAGS="$CLIP_DEBUG_PRG_FLAGS --verbose=$CLIP_DEBUG_PRG_FLAGS_VERBOSITY"
#CLIP_DEBUG_PRG_FLAGS="$CLIP_DEBUG_PRG_FLAGS --debug=$CLIP_DEBUG_PRG_FLAGS_DEBUG_LEVEL"
#CLIP_DEBUG_PRG_FLAGS="$CLIP_DEBUG_PRG_FLAGS --noremove-c"
[ -d $Clip_S_Dir ] || mkdir -p$V $Clip_S_Dir
echo $PrgVerbose 					>$Clip_S_Dir/PrgVerbose.setup.ini
echo $C_PRG_FLAGS 				>$Clip_S_Dir/C_PRG_FLAGS.setup.ini
#echo $C_DEBUG_PRG_FLAGS 		>$Clip_S_Dir/C_DEBUG_PRG_FLAGS.setup.ini
echo $CLIP_PRG_FLAGS 			>$Clip_S_Dir/CLIP_PRG_FLAGS.setup.ini
echo $CLIP_PRG_SHORT_FLAGS 	>$Clip_S_Dir/CLIP_PRG_SHORT_FLAGS.setup.ini
#echo $CLIP_DEBUG_PRG_FLAGS 	>$Clip_S_Dir/CLIP_DEBUG_PRG_FLAGS.setup.ini
echo $List 							>$Clip_S_Dir/prg.setup.ini
echo $List
