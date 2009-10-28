#include "ci_directry.ch"

Procedure Main ()
	Local cFilename := ""
	Local aInclude := {}
	Local aIFiles := {}
	Local n01ii := 1
	Local nParameters := 0
	Local cString := ""
	Local cString2 := ""
	Local aDbStruct := {}
	Local cHomeDir := ""
	Local aFileName := {}
	Local ToFind
	Local n02ii := 0
	Local nCurRec := 0
	aAdd ( aDbStruct, { "Line", "C", 2048, 0} )
	nParameters := pCount()
	Set Translate Off
	Set Console Off
	Set Print Off
	Set Device To Screen
	n01ii := 0
	AAdd ( aInclude, "")
	AAdd ( aInclude, "./")
	Do while ++n01ii <= nParameters
		cString := Param(n01ii)
*		OutErr ( "Param :", n01ii, cString )
		Do Case
		Case Len ( cString) > 2
			Do Case
			Case SubStr( cString, 1, 2 ) = "-I"
				aAdd( aInclude, SubStr( cString, 3 ) )
			Case SubStr( cString, 1, 1 ) = "-"
			EndCase
		Case cString = "-I"
			aAdd ( aInclude, Param( n01ii + 1 ) )
		EndCase
	EndDo
	cFilename := Param ( --n01ii )
	AAdd ( aFileName, cFilename )
*	For n01ii := 1 To Len (aInclude)
*		OutErr ( "-I", n01ii, aInclude[n01ii] )
*	next
	cHomeDir := GetEnv( "HOME")
	DbCreate( (cHomeDir + "/tmp/temp"), aDbStruct )
	Use (cHomeDir + "/tmp/temp") exclusive
	Go Top
	Set Delete On
	Zap
	OutErr ( Chr(13), "File : ", cFilename)
	if Right( cFilename, 4 ) = ".prg"
*		OutErr ( Chr(13), ".prg", cFilename)
		if File ( "./.cliprc" )
*			OutErr ( Chr(13), "./.cliprc" )
			Append from "./.cliprc" SDF
		EndIf
		if File ( cHomeDir + "/.cliprc" )
*			OutErr ( Chr(13), cHomeDir + "/.cliprc" )
			Append from ( cHomeDir + "/.cliprc" ) SDF
		EndIf
		Go Top
		Do While .not. Eof()
			cString := AllTrim ( Temp->line)
			if SubStr( cString, 1, 2 ) = "-U"
*				OutErr ( Chr(13), "-U", AllTrim ( Temp->line ))
				cString := '#include "' - AllTrim (SubStr ( cString, 3)) - '"'
				Temp->line := cString
			EndIf
*			OutErr ( Chr(13), "-U", AllTrim ( Temp->line ))
			Commit
			Skip
		EndDo
		Append Blank
		Temp->line := '#include "ci_clip.h"'
		Commit
		Append Blank
		Temp->line := '#include "ci_std.ch"'
		Commit
	EndIf
	n01ii := Len(cFileName)
	Do Case
	Case Right ( cFileName, 4 ) = ".lex"
		cString := "./" + SubStr ( cFileName, 1, n01ii - 4 ) + ".d"
	Case Right ( cFileName, 4 ) = ".prg"
		cString := "./" + SubStr ( cFileName, 1, n01ii - 4 ) + ".d"
	Case Right ( cFileName, 2 ) = ".c"
		cString := "./" + SubStr ( cFileName, 1, n01ii - 2 ) + ".d"
	Case Right ( cFileName, 2 ) = ".y"
		cString := "./" + SubStr ( cFileName, 1, n01ii - 2 ) + ".d"
	EndCase
*	OutErr ( Chr(13), "prt", cString)
	Set Printer To &( cString)
	For n02ii := 1 To Len ( aFileName )
		Set Console On
		cFilename := aFileName [ n02ii ]
		OutErr ( Chr(13), "Reading file :", cFilename )
		if File ( (cFilename) )
			Pack
			Append From &(cFilename) SDF
			Go Top
*			OutErr ( RecCount() )
			Do While .not. Eof()
				Set Console On
*				OutErr ( RecNo() )
				nCurRec := RecNo()
				if "#include" $ temp->line
					cString := AllTrim ( temp->line)
					Do While  Chr(8) $ cString
						cString := StrTran( cString, Chr(8), " " )
					EndDo
					Do While  Chr(9) $ cString
						cString := StrTran( cString, Chr(9), " " )
					EndDo
					cString := AllTrim ( cString )
*					OutErr ( Chr(10), chr(13), cString )
					if "#include" == SubStr ( cString, 1, 8 )
						cString := AllTrim ( SubStr ( cString, 9 ))
						nParameters := SubStr ( cString, 1, 1 )
						if .not. ( nParameters $ '"<')
							nParameters := " "
							cString := cString + " "
						EndIf
						if nParameters = "<"
							nParameters := ">"
						EndIf
						cString2 := ""
						cString := SubStr ( cString, 2)
						n01ii := Len (cString)
						Do While ( n01ii-- >= 0 ) .and. (SubStr ( cString, 1, 1) != nParameters)
							cString2 := cString2 + SubStr( cString, 1, 1)
							cString := SubStr ( cString, 2)
						EndDo
						If Len( cString) = 0 .or. n01ii < 0
							OutErr ( "Error on line :", AllTrim( Str( RecNo() ) ), rTrim( Temp->line))
							Go nCurRec
							Delete
							Skip
							loop
						EndIf
						cString2 := Lower ( cString2 )
*						OutErr ( Chr(10), chr(13), "cString2 :", cString2 )
						if ( cString2 != "ci_clipcfg.h")
							If AScan ( aIFiles, cString2) = 0
								AAdd (aIFiles, cString2)
*								OutErr ( "Ascan", cString2 )
								n01ii := 1
								ToFind := .T.
								Do While ( n01ii <= Len( aInclude) ) .and. ToFind
*									OutErr ( Chr(10), chr(13), "inc", aInclude [ n01ii ] )
									if n01ii > 1
										cString :=  aInclude [ n01ii ] + "/" + cString2
									Else
										cString := cString2
									EndIf
									If File ( (cString) )
										ToFind := .F.
*										OutErr ( Chr(10), chr(13), "File:", cString )
										Set Console On
										Set Print On
										OutErr ( Chr(13), cString )
										? cString
										AAdd ( aFileName, cString )
										Set Console Off
										Set Print Off
									EndIf
									++n01ii
								EndDo
								if .not. (File ( cString ) )
									OutErr ( Chr(13), "file ", cString2, " not found in :")
									For n01ii := 1 to Len (aInclude)
										OutErr ( Chr(13), aInclude [ n01ii ] )
									Next
								EndIf
							EndIf
						EndIf
					EndIf
				EndIf
				Go nCurRec
				Delete
				Skip
			EndDo
		EndIf
	Next
	Use
	OutErr ( Chr(13))
	fErase (cHomeDir + "temp.dbf" )
	Set Printer To
	Quit
