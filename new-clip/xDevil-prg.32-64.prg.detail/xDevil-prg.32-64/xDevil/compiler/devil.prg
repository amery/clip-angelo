/*
			main program of xDevil project packages.
			2010.02.10 Start
			Angelo Girardi
*/
#include <dv_xDevilcfg.h>
#include <dv_language.ch>
/*
// Variables defined as follow :
// axx 		array
// nxx 		numeric
// cxx 		character
// sxx 		string
// bxx 		boolean
// p_xx 		global parameter
// rxx 		block runtime
// vexx 		variable environment
// lxx		local
// pxx 		local parameter

*/
Static p_axDevilError := {}
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
Static p_sxDevilVersion := xDevil_VERSION
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
Static p_nxDevilVerbosity := 1 	// compiler verbosity level
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
Static vexDevil_CMDSTR := ""
Static vexDevilLIB := ""
Static vexDevilLIBS := ""
Static vexDevil_LOCALE_ROOT := ""
Static vexDevil_MODULE := ""
Static vexDevil_NAMES := ""
Static vexDevilROOT := ""
Static vexDevilSLIB := ""
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
Static vexDevil_LANG := ""
Static vexDevil_HOSTCS := ""
Static vexDevil_CLIENTCS := ""
Static vexDevil_LOCALE := ""
Static vexDevil_LOGLEVEL := ""
Static vexDevil_SCANMODE := ""
Static vexDevil_SCANSTART := ""
Static vexDevil_SCANSTOP := ""
Static vexDevil_KEYMAP := ""
Static vexDevil_LINECHARS := ""
Static vexDevil_DLINECHARS := ""
Static vexDevil_COLORMAP := ""
Static vexDevil_CHARSET := ""
Static vexDevil_LC_ALL := ""
Static vexDevil_TERM := ""

Function dv_xDevil
	Local laLineParameters := {}
	Local ln01ii := 0
	Local ln02ii := 0
	Local laxDevilRc := {}
	Local laxDevilEnv := {}
	Local laAllParameters
	Set LogLevel to 5
	For ln01ii := 0 to pCount()
		aAdd ( laLineParameters, Param ( ln01ii ) )
	next
	laxDevilRc := dv_ReadRC ()
	laxDevilEnv := dv_GetEnv ()
	For ln01ii := 1 To Len ( laxDevilRc ) Step 1
		aAdd ( laAllParameters, laxDevilRc [ ln01ii ] )
	Next
	For ln01ii := 1 To Len ( laxDevilEnv ) Step 1
		aAdd ( laAllParameters, laxDevilEnv [ ln01ii ] )
	Next
	For ln01ii := 1 To Len ( laLineParameters ) Step 1
		aAdd ( laAllParameters, laLineParameters [ ln01ii ] )
	Next
	dv_Parameter( laAllParameters )
	dv_DefaultInit()
	dv_Compile ( 0, .F. )
	For ln01ii := 1 To Len ( p_axDevilError) Step 1
		For ln02ii :=  1 To Len( p_axDevilError [ ln01ii] ) Step 1
			OutErr ( p_axDevilError [ ln01ii, ln02ii] )
		Next
		OutErr ( Chr(13))
	Next
	Quit

Static Procedure OpenFiles()
	Local laDbTemp := {	{"CONTENT", "C", 2048}, 	;
										{"LINENUM", "N", 10, 0}, 	;
										{"FILENAME", "C", 255, 0} }
	Local laDbDefine := {	{"CONTENT", "C", 2048}, 	;
								{"COMMAND", "C", 2048}, 	;
								{"LINENUM", "N", 10, 0}, 	;
								{"FILENAME", "C", 255, 0}, ;
								{"CTYPE", "C", 1, 0}  }
	Local laDbOutput := {	{"COMMAND", "C", 2048}, 	;
								{"LINENUM", "N", 10, 0}, 	;
								{"FILENAME", "C", 255, 0}, ;
								{"CTYPE", "C", 1, 0} }
	Local ln01ii := 0
	Local lsFileToRead := ""
	Local lnCurRecord := 0
	*
	If Empty ( p_sSourcePrg )
		aAdd ( p_axDevilError, { ERROR_SOURCE })
		dv_Abort()
	EndIf
	Request DbfCdx
	DbCreate ( (p_cHomeDir + "temp/temp"), laDbTemp )
	DbCreate ( (p_cHomeDir + "temp/define"), laDbDefine )
	DbCreate ( (p_cHomeDir + "temp/command"), laDbDefine )
	DbCreate ( (p_cHomeDir + "temp/xcommand"), laDbDefine )
	DbCreate ( (p_cHomeDir + "temp/output"), laDbStructure )
	Close DataBases
	Select 0
	use ( p_cHomeDir + "temp/temp" ) exclusive Alias temp
	Select 0
	use ( p_cHomeDir + "temp/define" ) exclusive Alias define
	Index On SubStr ( Field->Content, 1 , 200) to ( p_cHomeDir + "temp/define" )
*	Select 0
*	use ( cHomeDir + "temp/f_define" ) exclusive Alias define
*	Index On SubStr ( Field->Content, 1 , 200) to ( cHomeDir + "temp/f_define" )
	Select 0
	use ( cHomeDir + "temp/xdefine" ) exclusive Alias xdefine
	Index On SubStr ( Field->Content, 1 , 200) to ( cHomeDir + "temp/xdefine" )
	Select 0
	use ( p_cHomeDir + "temp/command" ) exclusive Alias command
	Index On SubStr ( Field->Content, 1 , 200) to ( p_cHomeDir + "temp/command" )
	Select 0
	use ( cHomeDir + "temp/xcommand" ) exclusive Alias xcommand
	Index On SubStr ( Field->Content, 1 , 200) to ( cHomeDir + "temp/xcommand" )
	Select 0
	use ( cHomeDir + "temp/translate" ) exclusive Alias translate
	Index On SubStr ( Field->Content, 1 , 200) to ( cHomeDir + "temp/translate" )
	Select 0
	use ( cHomeDir + "temp/xtranslate" ) exclusive Alias xtranslate
	Index On SubStr ( Field->Content, 1 , 200) to ( cHomeDir + "temp/xtranslate" )
	Select 0
	use ( p_cHomeDir + "temp/output" ) exclusive Alias output
	Select temp
	For ln01ii := 1 To Len ( p_aUseStdCh )
		lsFileToRead := p_aUseStdCh [ ln01ii ]
		If ( Upper ( lsFileToRead ) == "DV_STD.CH" ) .or. ( "/DV_STD.CH" $ Upper ( lsFileToRead ) )
			lnCurRecord := dv_stdch ( Temp->FileName, RecNo() + 1 )
		Else
			lnCurRecord := dv_ReadFile ( lsFileToRead, RecNo() + 1 )
		EndIf
	Next
	lnCurRecord := dv_ReadFile ( p_sSourcePrg,  RecNo() + 1 )
	Go lnCurRecord


#include "dv_abort.prg"
#include "dv_adddefine.prg"
#include "dv_addout.prg"
#include "dv_command.prg"
#include "dv_comment.prg"
#include "dv_compile.prg"
#include "dv_compline.prg"
#include "dv_defaultinit.prg"
#include "dv_define.prg"
#include "dv_getenv.prg"
#include "dv_include.prg"
#include "dv_line2array.prg"
#include "dv_parameter.prg"
#include "dv_readfile.prg"
#include "dv_readrc.prg"
#include "dv_rmtab.prg"
#include "dv_set.prg"
#include "dv_stdch.prg"
#include "dv_translate.prg"
#include "dv_undef.prg"
#include "dv_warning.prg"
#include "dv_xcommand.prg"
#include "dv_xdefine.prg"
#include "dv_xtranslate.prg"
