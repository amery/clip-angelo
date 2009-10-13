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
	Local n02ii := 0
	Local nCurRec := 0
	aAdd ( aDbStruct, { "Line", "C", 2048, 0} )
	nParameters := pCount()
	Set Translate Off
	Set Console On
	Set Print Off
	Set Device To Screen
	n01ii := 0
	AAdd ( aInclude, ".")
	Do while ++n01ii <= nParameters
		cString := Param(n01ii)
		? "Par :", n01ii, cString
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
	If cFilename = "commands.prg"
		AltD(1)
	EndIf
*	aFileName := Directory( "./*.prg", "HDRS")
	AAdd ( aFileName, cFilename )
	For n01ii := 1 To Len (aInclude)
		? "I", n01ii, aInclude[n01ii]
	next
	cHomeDir := GetEnv( "HOME")
	DbCreate( (cHomeDir + "/tmp/temp"), aDbStruct )
	Use (cHomeDir + "/tmp/temp") exclusive
	Set Delete On
	Zap
	For n02ii := 1 To Len ( aFileName )
		cFilename := aFileName [ n02ii ]
		? "File :", cFilename
		if File ( (cFilename) )
			Append From (cFilename) SDF
			Go Top
			?? RecCount()
			Do While .not. Eof()
				nCurRec := RecNo()
				if "#include" $ temp->line
					cString := AllTrim ( temp->line)
					Do While  Chr(8) $ cString
						cString := StrTran( cString, Chr(8), " " )
					EndDo
					Do While  Chr(9) $ cString
						cString := StrTran( cString, Chr(9), " " )
					EndDo
					? cString
					if "#include" == SubStr (cString, 1, 8)
						cString := AllTrim( SubStr ( cString, 9))
						nParameters := SubStr( cString, 1, 1 )
						if nParameters = "<"
							nParameters := ">"
						EndIf
						cString2 := ""
						cString := SubStr ( cString, 2)
						Do While SubStr ( cString, 1, 1) != nParameters
							cString2 := cString2 + SubStr( cString, 1, 1)
							cString := SubStr ( cString, 2)
							If Len( cString) = 0
								? "Error on line :", AllTrim( Str( RecNo() ) ), rTrim( Temp->line)
								Zap
								Quit
							EndIf
						EndDo
						cString2 := Lower ( cString2 )
						? "cString2 :", cString2
						If AScan ( aIFiles, cString2) = 0
							AAdd (aIFiles, cString2)
							? "Ascan"
							For n01ii := 1 To Len( aInclude)
								cString :=  aInclude [ n01ii ] + "/" + cString2
								If File ( (cString) )
									? cString
									AAdd ( aFileName, cString )
								EndIf
							Next
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
	fErase (cHomeDir + "temp.dbf" )
	Quit
