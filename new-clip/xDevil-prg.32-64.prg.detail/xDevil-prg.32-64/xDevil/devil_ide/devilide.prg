/*
#
# 	Author : 	Angelo GIRARDI
# 					http://www.geocities.com/agag00012
#					angelo.girardi@swing.be
#
#				This is free program under GNU free GPL
#
#				This program will try to have a friendly interface to clip development package.
#
#
#	1st Release : 	200/01/26
#

*/

#include "directry.ch"

Static aFileName
Static aDirName
Static aSettings


Function main ()

	local aDirList
	local aFileList
	local cEnvironment
	local aSettingsFileStructure
	local sSettingsDbfFile
	local sSettingsIdxFile
	local aClipWordFileStructure
	local sClipWordDbfFile
	local sClipWordIdxFile
	local sPathClipWord
	local CLIP_PRG_DIR
	Set Translate Path Off
	Set Century On
	RddDefault( "CDX")
	CLIP_PRG_DIR=GetEnv("CLIP_PRG_DIR")
	Set Default To &(sEnvironment)
	sEnvironment := 
	sPathSettings := "clip-devel/"
	sSettingsDbfFile := "filesettings.dbf"
	sSettingsIdxFile := "filesettings.cdx"
	Add { "SDIRNAME", 	"C", 	99, 	0 } to Array aSettingsFileStructure
	Add { "SFILENAME", 	"C", 	50, 	0 } to Array aSettingsFileStructure
	Add { "SFILETYPE", 	"C", 	10, 	0 } to Array aSettingsFileStructure
	Add { "SSETTINGS", 	"C", 	100, 	0 } to Array aSettingsFileStructure
	
	if .not. File ( &(sDbfFile))
		then
		dbCreate ( $(sDbfFile), aFileStructure )
		use
	EndIf
	use &(sFileName) New Shared
	if .not. Used()
		then
		Wait "File "+ sDbfFile +" locked or unreadable"
		quit
	EndIf
	if File( sIdxFile )
		set Index to &( sIdxFile )
	else
		Index on filesettings->SDIRNAME + filesettings->sFILENAME to &( sIdxFile )
	EndIf
	
	sDbfFile := "../clip-devel/clipwords.dbf"
	aSize( aFileStructure, 0 )
	
	Add { "NAME", 	"C", 	64, 	0 } to Array aFileStructure
	Select 0
	if .not. File ( &( sDbfFile ) )
		then
		dbCreate ( $( sDbfFile ), aFileStructure )
		use
	EndIf
	use &( sFileName ) New Shared
	if .not. Used()
		then
		Wait "File "+ sDbfFile +" locked or unreadable"
		quit
	EndIf
	if File( sIdxFile )
		set Index to &( sIdxFile )
	else
		Index on Upper( clipwords->NAME ) to &( sIdxFile )
	EndIf
	
	* Delete All
	* Pack
	ReaFile ( "" )
	ReadFile ("clip")
	ReadFile ("cliplib")
	ReadFile ("prg")
	Do While .T.

	EndDo
	


Function ReadFile ( cDirName )
	local aFileList
	local nii
	local njj
	local aDirList
	local cString
	local cType
	
	? "Reading :", cDirname
	aDirList := Directory( "*.*", "HSD" )
	aDirList := SortDirList ( aDirList )
	njj := aSize(aDirList)
	for nii := 1 to njj Step 1
		cType := FileType( aDirList[nii, F_NAME ], aDirList[nii, F_ATTR ] )
		cString := sDirName  + cType + aDirList[nii, F_NAME]
		Seek &( cString)
		if .not. Found()
			Append Blank
			clipsettings->sDirName := sDirName
			clipsettings->sFileName := aDirList[nii, F_NAME]
			clipsettings->sFileType := cType
		EndIf
	Next
	
Static Function FileType ( cFileName, cFileAttr )
	local cString := ""
	local cExtension := ""
	local cReturn := ""
	
	If At( ".", cFileName) = 0
		cExtension := ""
	else
		cExtension := cFileName
	EndIf
	Do while At( ".", cExtension != 0 )
		cExtension := SubStr( cExtension, At( ".", cExtension) + 1)
	EndDo
	
	Do case
	case cFileAttr = "D"
		cReturn := "D" + "." + cExtension
	case cFileAttr = "H"
		cReturn := "H" + "." + cExtension
	case cFileAttr = "S"
		cReturn := "S" + "." + cExtension
	Otherwise
		cReturn := "r" + "." + cExtension
	EndCase
	return PadL(cReturn, 10)
	

Static Function SortDirList ( cDirList )
   Local c01FileName
   Local c02FileName
   Local cFileName
   Local cFileDate
   Local cFileTime
   Local cFileAttr
   Local cFileSize
   Local n01ii := 0
   Local n02ii := 0
   Local n03ii := 0
	Local n04ii := 0
	Local bToSort := .T.
	Local bToUpSort := .T.
	Local bToDownSort := .T.
	Local aTemp
	Local bEval
	/*
	bEval:={|x,y| Upper(x)>Upper(y)}
	ASORT(cDirList,,,bEval)
	Return cDirList
	*/
	aTemp := Array( 5 )
  
   n02ii := Len ( cDirList )
	n01ii := 0
	@ Row(), 40 Say n02ii
	Do While bToSort
		bToSort := .F.
		++n01ii
		n03ii := n01ii
		@ Row(), 10 Say n01ii
		@ Row(), 30 Say n02ii
		--n02ii
		bToDownSort := .F.
		Do While (n03ii < n02ii) .and. bToUpSort
			// @ Row(), Col() Say n02ii
			// @ Row(), Col() Say n03ii
         c01FileName := cDirList[n03ii, F_NAME ]
	      c02FileName := cDirList[n03ii + 1, F_NAME ]
	      If ( c01FileName ) > ( c02FileName )
				/*
				aTemp := aCopy( cDirList[n03ii] )
				cDirList[n03ii] := aCopy( cDirList[ n03ii + 1 ] )
				cDirList[ n03ii + 1 ] := aCopy( cDirList[ n03ii ] )
				*/
				*
	         cFileName := cDirList[n03ii, F_NAME ]
	         cDirList[ n03ii , F_NAME] := cDirList[ n03ii + 1, F_NAME ]
	         cDirList[ n03ii + 1, F_NAME ] := cFileName
	         *
	         cFileDate := cDirList [n03ii, F_DATE ]
	         cDirList [ n03ii , F_DATE] := cDirList [ n03ii + 1, F_DATE ]
	         cDirList [ n03ii + 1, F_DATE ] := cFileDate
            *
	         cFileTime := cDirList [n03ii, F_TIME ]
	         cDirList [ n03ii , F_TIME] := cDirList [ n03ii + 1, F_TIME ]
	         cDirList [ n03ii + 1, F_TIME ] := cFileTime
	         *
	         cFileAttr := cDirList [n03ii, F_ATTR ]
	         cDirList [ n03ii , F_ATTR] := cDirList [ n03ii + 1, F_ATTR ]
	         cDirList [ n03ii + 1, F_ATTR ] := cFileAttr
				*
	         cFileSize := cDirList [n03ii, F_SIZE ]
	         cDirList [ n03ii , F_SIZE ] := cDirList [ n03ii + 1, F_SIZE ]
	         cDirList [ n03ii + 1, F_SIZE ] := cFileSize
				*/
				bToSort := .T.
				bToDownSort := .T.
	      EndIf
			++ n03ii
      EndDo
		@ Row(), 30 Say n02ii
		n03ii := n02ii
		bToDownSort := .F.
		Do While ( n03ii > n01ii) .and. bToDownSort
			// @ Row(), Col() Say n02ii
			// @ Row(), Col() Say n03ii
        	c01FileName := cDirList[n03ii, F_NAME ]
      	c02FileName := cDirList[n03ii - 1, F_NAME ]
      	If ( c01FileName ) < ( c02FileName )
				/*
				aTemp := aCopy( cDirList[n03ii] )
				cDirList[n03ii] := aCopy( cDirList[ n03ii - 1 ] )
				cDirList[ n03ii - 1 ] := aCopy( cDirList[ n03ii ] )
				*/
				*
      	   cFileName := cDirList[n03ii, F_NAME ]
         	cDirList[ n03ii , F_NAME] := cDirList[ n03ii - 1, F_NAME ]
         	cDirList[ n03ii - 1, F_NAME ] := cFileName
         	*
         	cFileDate := cDirList [n03ii, F_DATE ]
         	cDirList [ n03ii , F_DATE] := cDirList [ n03ii - 1, F_DATE ]
         	cDirList [ n03ii - 1, F_DATE ] := cFileDate
           	*
         	cFileTime := cDirList [n03ii, F_TIME ]
         	cDirList [ n03ii , F_TIME] := cDirList [ n03ii - 1, F_TIME ]
         	cDirList [ n03ii - 1, F_TIME ] := cFileTime
         	*
         	cFileAttr := cDirList [n03ii, F_ATTR ]
         	cDirList [ n03ii , F_ATTR] := cDirList [ n03ii - 1, F_ATTR ]
         	cDirList [ n03ii - 1, F_ATTR ] := cFileAttr
				*
	         cFileSize := cDirList [n03ii, F_SIZE ]
	         cDirList [ n03ii , F_SIZE ] := cDirList [ n03ii - 1, F_SIZE ]
	         cDirList [ n03ii - 1, F_SIZE ] := cFileSize
				*/
				bToSort := .T.
				bToUpSort := .T.
      	EndIf
			--n03ii
     	EndDo
		--n02ii
   EndDo
Return ( cDirList )

	