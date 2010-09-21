#!/bin/bash -u
#
# initiates or updates CC & clip compilers parameters for clip dir
#
source $Clip_F_F
export C_CLIP_FLAGS_ADDITIONNAL=" "
export CLIP_CLIP_FLAGS_ADDITIONNAL=" "
export List=""

[ -f $Clip_S_Dir/clip.setup.ini ] && export List=$(cat $Clip_S_Dir/clip.setup.ini )

[ -z $List ] && 	export List="
		C_FLAGS_GEN
		C_CLIP_FLAGS
		CLIP_FLAGS_GEN
		CLIP_CLIP_FLAGS
		CLIP_FLAGS_GEN_SHORT
		CLIP_CLIP_SHORT_FLAGS
		ClipVerbose
	"
	#"	CLIP_DEBUG_CLIP_FLAGS
	#	C_DEBUG_CLIP_FLAGS
	#	"
	export ClipVerbose="v"
	for nii in $List ; do
	if [ -f $Clip_S_Dir/$nii.setup.ini ] ; then
		export $nii="$(cat $Clip_S_Dir/$nii.setup.ini)"
	else
		export ClipVerbose="-"
	fi
done

if [ $ClipVerbose == "-" ] ; then
	trt=n
	while [[ "$trt" = [Nn] ]] ; do
		OnScreen 10 'Configuration of clip, tools & libraries'
		OnScreen 1 'Some parameters need to be given to suit your needs'
		OnScreen 1 "While compiling 'clip' subdirectories"
		OnScreen 2 "    cp -f\$V jklm (command is verbose) in batch files or cp -f\$(V) in Makefile"
		export ClipVerbose=$(ReadVarX_Min_Max						 	" Desable '-v' in command(s) 	(suggested 0) : " 	0 1 	"v" "/" )
#		OnScreen 3 "For NON DEBUG version mode : 'Production'"
		OnScreen 2 "For CC compiling general : $CC $C_FLAGS_GEN source.c"
		OnScreen 0
		export C_CLIP_FLAGS_VERBOSITY=$(ReadVarX_Min_Max 			" CC verbosity '--verbose' 	(suggested 0) : " 	0 1 	"/" '--verbose' )
		OnScreen 0
		export C_CLIP_FLAGS_DEBUG_LEVEL=$(ReadVarX_Min_Max 			" CC debug '-gN'					(suggested 0) : " 	0 3 	0 1 2 3 )
		OnScreen 0
		export C_CLIP_FLAGS_OPTIMISE_LEVEL=$(ReadVarX_Min_Max 		" CC optimise -On					(suggested 0) : " 	0 3 	0 1 2 3 )
		OnScreen 0
		export C_CLIP_FLAGS_ADDITIONNAL=" "
		OnScreen 1
		read -p "Any additionnal parameter(s) : " C_CLIP_FLAGS_ADDITIONNAL >&0
		OnScreen 2 "For Clip compiling general : clip $CLIP_FLAGS_GEN source.prg"
		OnScreen 0
		export CLIP_CLIP_FLAGS_VERBOSITY=$(ReadVarX_Min_Max 		" clip verbosity 					(suggested 0) : " 	0 6 	0 1 2 3 4 5 6 )
		OnScreen 0
		export CLIP_CLIP_FLAGS_DEBUG_LEVEL=$(ReadVarX_Min_Max 		" clip debug 						(suggested 0) : " 	0 1 	0 1 )
		OnScreen 0
		export CLIP_CLIP_FLAGS_OPTIMISE_LEVEL=$(ReadVarX_Min_Max 	" clip optimise  					(suggested 0) : " 	0 3 	0 1 2 3 )
		OnScreen 0
		export CLIP_CLIP_FLAGS_ADDITIONNAL=" "
		OnScreen 1
		read -p "Any additionnal parameter(s) : " CLIP_CLIP_FLAGS_ADDITIONNAL >&0
#		OnScreen 3 "For DEBUG || TEST version mode :"
#		OnScreen 1 "For CC compiling :"
#		C_DEBUG_CLIP_FLAGS_VERBOSITY=$(ReadVarX_Min_Max 	" CC verbosity 					(suggested 1) : " 	0 1 	"/" "--verbose" )
#		C_DEBUG_CLIP_FLAGS_DEBUG_LEVEL=3
#		C_DEBUG_CLIP_FLAGS_OPTIMISE_LEVEL=0
#		OnScreen 2 "For Clip compiling :"
#		CLIP_DEBUG_CLIP_FLAGS_VERBOSITY=$(ReadVarX_Min_Max " clip verbosity 					(suggested 2) : " 	0 6 	0 1 2 3 4 5 6 )
#		CLIP_DEBUG_CLIP_FLAGS_DEBUG_LEVEL=1
#		CLIP_DEBUG_CLIP_FLAGS_OPTIMISE_LEVEL=0
		# CC compiler's flags
		export C_CLIP_FLAGS="$C_FLAGS_GEN $C_CLIP_FLAGS_VERBOSITY"
		export C_CLIP_FLAGS="$C_CLIP_FLAGS -O$C_CLIP_FLAGS_OPTIMISE_LEVEL"
		export C_CLIP_FLAGS="$C_CLIP_FLAGS -g$C_CLIP_FLAGS_DEBUG_LEVEL"
		export C_CLIP_FLAGS="$C_CLIP_FLAGS $C_CLIP_FLAGS_ADDITIONNAL"
		# clip compiler's flags
		export CLIP_CLIP_SHORT_FLAGS="$CLIP_FLAGS_GEN_SHORT -o=$CLIP_CLIP_FLAGS_OPTIMISE_LEVEL"
		export CLIP_CLIP_SHORT_FLAGS="$CLIP_CLIP_SHORT_FLAGS --verbose=$CLIP_CLIP_FLAGS_VERBOSITY"
		export CLIP_CLIP_SHORT_FLAGS="$CLIP_CLIP_SHORT_FLAGS -g=$CLIP_CLIP_FLAGS_DEBUG_LEVEL"
		export CLIP_CLIP_SHORT_FLAGS="$CLIP_CLIP_SHORT_FLAGS $CLIP_CLIP_FLAGS_ADDITIONNAL"
		#
		export CLIP_CLIP_FLAGS="$CLIP_FLAGS_GEN --optimise=$CLIP_CLIP_FLAGS_OPTIMISE_LEVEL"
		export CLIP_CLIP_FLAGS="$CLIP_CLIP_FLAGS --verbose=$CLIP_CLIP_FLAGS_VERBOSITY"
		export CLIP_CLIP_FLAGS="$CLIP_CLIP_FLAGS --debug=$CLIP_CLIP_FLAGS_DEBUG_LEVEL"
		export CLIP_CLIP_FLAGS="$CLIP_CLIP_FLAGS $CLIP_CLIP_FLAGS_ADDITIONNAL"
		##  for test purposes
		# CC compiler's flags
#		C_DEBUG_CLIP_FLAGS="$C_FLAGS_GEN_DEBUG $C_DEBUG_CLIP_FLAGS_VERBOSITY"
#		C_DEBUG_CLIP_FLAGS="$C_DEBUG_CLIP_FLAGS -O$C_DEBUG_CLIP_FLAGS_OPTIMISE_LEVEL"
#		C_DEBUG_CLIP_FLAGS="$C_DEBUG_CLIP_FLAGS -g$C_DEBUG_CLIP_FLAGS_DEBUG_LEVEL"
		# clip compiler's flags
#		CLIP_DEBUG_CLIP_FLAGS="$CLIP_FLAGS_GEN_DEBUG --optimise=$CLIP_DEBUG_CLIP_FLAGS_OPTIMISE_LEVEL"
#		CLIP_DEBUG_CLIP_FLAGS="$CLIP_DEBUG_CLIP_FLAGS --verbose=$CLIP_DEBUG_CLIP_FLAGS_VERBOSITY"
#		CLIP_DEBUG_CLIP_FLAGS="$CLIP_DEBUG_CLIP_FLAGS --debug=$CLIP_DEBUG_CLIP_FLAGS_DEBUG_LEVEL"
#		CLIP_DEBUG_CLIP_FLAGS="$CLIP_DEBUG_CLIP_FLAGS --noremove-c "
		OnScreen 10 "Value of set variables for compiling 'clip' subdirectories"
		OnScreen 2 "For cmd e.g : rm -f$ClipVerbose"
#		OnScreen 2 "For NON DEBUG version mode : 'Production'"
		OnScreen 1 "For CC compiling :"
		OnScreen 1 "  cc command line will be like :"
		OnScreen 1 "\$ CC $C_CLIP_FLAGS source.c"
		OnScreen 1 " For Clip compiling :"
		OnScreen 1 "  clip command line will be like :"
		OnScreen 1 "\$ clip $CLIP_CLIP_FLAGS source.prg"
#		OnScreen 2 "For DEBUG || TEST version mode :"
#		OnScreen 1 " For CC compiling :"
#		OnScreen 1 "  cc command line will be like :"
#		OnScreen 1 "\$ $CC $C_DEBUG_CLIP_FLAGS source.c"
#		OnScreen 1 " For Clip compiling :"
#		OnScreen 1 "  clip command line will be like :"
#		OnScreen 1 "\$ clip $CLIP_DEBUG_CLIP_FLAGS source.prg"
		OnScreen 5
		read -n 1 -p "Is that what you need? [ Y/N ] : " trt >&0
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
if ! [[ $C_CLIP_FLAGS_VERBOSITY == --verbose ]] ; then
	export C_CLIP_FLAGS_VERBOSITY=""
fi
#if ! [[ $C_DEBUG_CLIP_FLAGS_VERBOSITY == --verbose ]] ; then
#	C_DEBUG_CLIP_FLAGS_VERBOSITY=""
#fi
		export C_CLIP_FLAGS="$C_FLAGS_GEN 		$C_CLIP_FLAGS_VERBOSITY"
		export C_CLIP_FLAGS="$C_CLIP_FLAGS 	-O$C_CLIP_FLAGS_OPTIMISE_LEVEL"
		export C_CLIP_FLAGS="$C_CLIP_FLAGS 	-g$C_CLIP_FLAGS_DEBUG_LEVEL"
		export C_CLIP_FLAGS="$C_CLIP_FLAGS 	$C_CLIP_FLAGS_ADDITIONNAL"
		# clip compiler's flags
		export CLIP_CLIP_FLAGS="$CLIP_FLAGS_GEN --optimise=$CLIP_CLIP_FLAGS_OPTIMISE_LEVEL"
		export CLIP_CLIP_FLAGS="$CLIP_CLIP_FLAGS --verbose=$CLIP_CLIP_FLAGS_VERBOSITY"
		export CLIP_CLIP_FLAGS="$CLIP_CLIP_FLAGS --debug=$CLIP_CLIP_FLAGS_DEBUG_LEVEL"
		export CLIP_CLIP_FLAGS="$CLIP_CLIP_FLAGS $CLIP_CLIP_FLAGS_ADDITIONNAL"
##  for test purposes
## CC compiler's flags
#C_DEBUG_CLIP_FLAGS="$C_FLAGS_GEN_DEBUG $C_DEBUG_CLIP_FLAGS_VERBOSITY"
#C_DEBUG_CLIP_FLAGS="$C_DEBUG_CLIP_FLAGS -O$C_DEBUG_CLIP_FLAGS_OPTIMISE_LEVEL"
#C_DEBUG_CLIP_FLAGS="$C_DEBUG_CLIP_FLAGS -g$C_DEBUG_CLIP_FLAGS_DEBUG_LEVEL"
# clip compiler's flags
#CLIP_DEBUG_CLIP_FLAGS="$CLIP_FLAGS_GEN_DEBUG --optimise=$CLIP_DEBUG_CLIP_FLAGS_OPTIMISE_LEVEL"
#CLIP_DEBUG_CLIP_FLAGS="$CLIP_DEBUG_CLIP_FLAGS --verbose=$CLIP_DEBUG_CLIP_FLAGS_VERBOSITY"
#CLIP_DEBUG_CLIP_FLAGS="$CLIP_DEBUG_CLIP_FLAGS --debug=$CLIP_DEBUG_CLIP_FLAGS_DEBUG_LEVEL"
#CLIP_DEBUG_CLIP_FLAGS="$CLIP_DEBUG_CLIP_FLAGS --noremove-c"
[ -d $Clip_S_Dir ] || mkdir -p$V $Clip_S_Dir
echo $ClipVerbose 				>$Clip_S_Dir/ClipVerbose.setup.ini
echo $C_CLIP_FLAGS 				>$Clip_S_Dir/C_CLIP_FLAGS.setup.ini
#echo $C_DEBUG_CLIP_FLAGS 		>$Clip_S_Dir/C_DEBUG_CLIP_FLAGS.setup.ini
echo $CLIP_CLIP_FLAGS 			>$Clip_S_Dir/CLIP_CLIP_FLAGS.setup.ini
echo $CLIP_CLIP_SHORT_FLAGS 	>$Clip_S_Dir/CLIP_CLIP_SHORT_FLAGS.setup.ini
#echo $CLIP_DEBUG_CLIP_FLAGS 	>$Clip_S_Dir/CLIP_DEBUG_CLIP_FLAGS.setup.ini
echo $List 							>$Clip_S_Dir/clip.setup.ini
echo $List
