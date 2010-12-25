*#include <cn_std.ch>
#include <cn_language.ch>
#include <cn_clipcfg.h>
/*
// Variables defined as follow :
// a 		array
// n 		numeric
// c 		character
// s 		string
// b 		boolean
// p_ 	parameters
// r 		block runtime
*/
Static p_aClipError := {}
* 		// array of compiling error
Static p_aParameters := {}
* 		// array of parameters
Static p_aSCommand := {}
* 			// array for command statements
Static p_aSDefine := {}
* 			// array for define statements
Static p_aSxTranslate := {} 		// array for xtranslate statements
Static p_aSxCommand := {} 		// array for xcommand statements
Static p_aSxDefine :={} 			//	array for xdefine statements
Static p_cHomeDir := ""
Static p_sClipVersion := CLIP_VERSION
Static p_aDefine := {}
Static p_aInclude := {}
Static p_aLibrary := {}
Static p_aObjs := {}
Static p_aUseStdCh := {}
Static p_aVariables := {}
Static p_bAsWord := .F.
Static p_bAutoMemVar := .F.
Static p_bcCode := .F.
Static p_bCompile := .F.
Static p_bDebug := .F.
Static p_bExec := .F.
Static p_bFullStatic := .F.
Static p_bMain := .F.
Static p_bNames := .F.
Static p_bNameSpaceWaring := .F.
Static p_bNoMain := .F.
Static p_bNoRemoveC := .T.
Static p_bpCode := .F.
Static p_bPreprocess := .F.
Static p_bShared := .F.
Static p_bSharedExec := .F.
Static p_bStop := .F. 			// abort compiling
Static p_bShowCommand := .F.
Static p_bShowSyntaxTree := .F.
Static p_nClipVerbosity := 1 	// compiler verbosity level
Static p_nOptimise := -1
Static p_sOutDir := ""
Static p_sOutPut := ""
Static p_sSourcePrg := ""
Static p_sSourceCharset := ""
Static p_sTargetCharset := ""
Static veCC := ""
Static veCDBG := ""
Static veCFLAGS := ""
Static veCHARSET := ""
Static veCLIP_CMDSTR := ""
Static veCLIPLIB := ""
Static veCLIPLIBS := ""
Static veCLIP_LOCALE_ROOT := ""
Static veCLIP_MODULE := ""
Static veCLIP_NAMES := ""
Static veCLIPROOT := ""
Static veCLIPSLIB := ""
Static veCOMPILE_FLAG := ""
Static veCOPT := ""
Static veINCLUDE_FLAG := ""
Static veLANG := ""
Static veLC_ALL := ""
Static veTERM := ""
Static veLIBSUF := ""
Static veOBJSUF := ""
Static veOUT_FLAG := ""
Static veSFLAGS := ""
Static veSLIBSUF := ""
Static veSOBJSUF := ""
Static veCLIP_LANG := ""
Static veCLIP_HOSTCS := ""
Static veCLIP_CLIENTCS := ""
Static veCLIP_LOCALE := ""
Static veCLIP_LOGLEVEL := ""
Static veCLIP_SCANMODE := ""
Static veCLIP_SCANSTART := ""
Static veCLIP_SCANSTOP := ""
Static veCLIP_KEYMAP := ""
Static veCLIP_LINECHARS := ""
Static veCLIP_DLINECHARS := ""
Static veCLIP_COLORMAP := ""
Static veCLIP_CHARSET := ""
Static veCLIP_LC_ALL := ""
Static veCLIP_TERM := ""

Function cn_Clip
	Local laLineParameters := {}
	Local ln01ii := 0
	Local ln02ii := 0
	Local laClipRc := {}
	Local laClipEnv := {}
	Local laAllParameters
	Set LogLevel to 5
	For ln01ii := 0 to pCount()
		aAdd ( laLineParameters, Param ( ln01ii ) )
	next
	laClipRc := cn_ReadRC ()
	laClipEnv := cn_GetEnv ()
	For ln01ii := 1 To Len ( laClipRc ) Step 1
		aAdd ( laAllParameters, laClipRc [ ln01ii ] )
	Next
	For ln01ii := 1 To Len ( laClipEnv ) Step 1
		aAdd ( laAllParameters, laClipEnv [ ln01ii ] )
	Next
	For ln01ii := 1 To Len ( laLineParameters ) Step 1
		aAdd ( laAllParameters, laLineParameters [ ln01ii ] )
	Next
	cn_Parameter( laAllParameters )
	cn_DefaultInit()
	cn_Compile ( 0, .F. )
	For ln01ii := 1 To Len ( p_aClipError) Step 1
		For ln02ii :=  1 To Len( p_aClipError [ ln01ii] ) Step 1
			OutErr ( p_aClipError [ ln01ii, ln02ii] )
		Next
		OutErr ( Chr(13))
	Next
	Quit

Static Procedure OpenFiles()
	Local laDbStructure := {	{"CONTENT", "C", 2048}, 	;
										{"LINENUM", "N", 6, 0}, 	;
										{"FILENAME", "C", 255, 0} }
	Local ln01ii := 0
	Local lsFileToRead := ""
	Local lnCurRecord := 0
	*
	If Empty ( p_sSourcePrg )
		aAdd ( p_aClipError, { ERROR_SOURCE })
		cn_Abort()
	EndIf
	Request DbfCdx
	DbCreate ( (p_cHomeDir + "temp/temp"), laDbStructure )
	laDbStructure := {	{"CONTENT", "C", 2048}, 	;
								{"COMMAND", "C", 2048}, 	;
								{"LINENUM", "N", 6, 0}, 	;
								{"FILENAME", "C", 255, 0}, ;
								{"CTYPE", "C", 1, 0}  }
	DbCreate ( (p_cHomeDir + "temp/define"), laDbStructure )
	DbCreate ( (p_cHomeDir + "temp/command"), laDbStructure )
	DbCreate ( (p_cHomeDir + "temp/xcommand"), laDbStructure )
	laDbStructure := {	{"COMMAND", "C", 2048}, 	;
								{"LINENUM", "N", 6, 0}, 	;
								{"FILENAME", "C", 255, 0}, ;
								{"CTYPE", "C", 1, 0} }
	DbCreate ( (p_cHomeDir + "temp/output"), laDbStructure )
	Close DataBases
	Select 0
	use ( p_cHomeDir + "temp/temp" ) exclusive Alias temp
	Select 0
	use ( p_cHomeDir + "temp/define" ) exclusive Alias define
	Index On SubStr ( Field->Content, 1 , 200) to ( p_cHomeDir + "temp/define" )
	Select 0
*	use ( cHomeDir + "temp/f_define" ) exclusive Alias define
*	Index On SubStr ( Field->Content, 1 , 200) to ( cHomeDir + "temp/f_define" )
*	Select 0
*	use ( cHomeDir + "temp/xdefine" ) exclusive Alias xdefine
*	Index On SubStr ( Field->Content, 1 , 200) to ( cHomeDir + "temp/xdefine" )
	Select 0
	use ( p_cHomeDir + "temp/command" ) exclusive Alias command
	Index On SubStr ( Field->Content, 1 , 200) to ( p_cHomeDir + "temp/command" )
*	Select 0
*	use ( cHomeDir + "temp/xcommand" ) exclusive Alias xcommand
*	Index On SubStr ( Field->Content, 1 , 200) to ( cHomeDir + "temp/xcommand" )
*	Select 0
*	use ( cHomeDir + "temp/translate" ) exclusive Alias translate
*	Index On SubStr ( Field->Content, 1 , 200) to ( cHomeDir + "temp/translate" )
*	Select 0
*	use ( cHomeDir + "temp/xtranslate" ) exclusive Alias xtranslate
*	Index On SubStr ( Field->Content, 1 , 200) to ( cHomeDir + "temp/xtranslate" )
	Select 0
	use ( p_cHomeDir + "temp/output" ) exclusive Alias output
	Select temp
	For ln01ii := 1 To Len ( p_aUseStdCh )
		lsFileToRead := p_aUseStdCh [ ln01ii ]
		If ( Upper ( lsFileToRead ) == "CN_STD.CH" ) .or. ( "/CN_STD.CH" $ Upper ( lsFileToRead ) )
			lnCurRecord := cn_stdch ( Temp->FileName, RecNo() + 1 )
		Else
			lnCurRecord := cn_ReadFile ( lsFileToRead, RecNo() + 1 )
		EndIf
	Next
	lnCurRecord := cn_ReadFile ( p_sSourcePrg,  RecNo() + 1 )
	Go lnCurRecord


#include "cn_abort.prg"
#include "cn_adddefine.prg"
#include "cn_addout.prg"
#include "cn_command.prg"
#include "cn_comment.prg"
#include "cn_compile.prg"
#include "cn_compline.prg"
#include "cn_defaultinit.prg"
#include "cn_define.prg"
#include "cn_getenv.prg"
#include "cn_include.prg"
#include "cn_line2array.prg"
#include "cn_parameter.prg"
#include "cn_readfile.prg"
#include "cn_readrc.prg"
#include "cn_rmtab.prg"
#include "cn_set.prg"
#include "cn_stdch.prg"
#include "cn_translate.prg"
#include "cn_undef.prg"
#include "cn_warning.prg"
#include "cn_xcommand.prg"
#include "cn_xdefine.prg"
#include "cn_xtranslate.prg"
