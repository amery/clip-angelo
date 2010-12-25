/*
    cn_compile.prg

*/
Procedure Cn_Compile ( lnFirst, pbCollect )
	Static saDefine := {}
	Local ln01ii := 0
	Local ln02ii := 0
	Local lnCurRecord := 0
	Local lsString := ""
	Local lsToSearch := ""
	Default pbCollect To .F.
	Do While .not. Eof()
		Do While .not. Eof()
			lsString := AllTrim( cn_RmTab ( Field->Content ) )
			Do While rAt( lsString, 1 ) == ";"
				Delete
				lsString := SubStr( lsString, 1, Len(lsString) - 1 )
				Skip
				lsString := AllTrim ( lsString ) + " " + AllTrim ( cn_RmTab ( AllTrim ( Field->Content ) ) )
			EndDo
			Field->Content := lsString
			Do Case
			Case Upper ( SubStr ( lsString, 1, 8 ) ) == "#INCLUDE"
				cn_Include ()
			Case Upper ( SubStr ( lsString, 1, 7 ) ) == "#DEFINE"
				cn_Define()
			Case Upper ( SubStr ( lsString, 1, 6 ) ) == "#UNDEF"
				cn_UnDef()
			Case Upper ( SubStr ( lsString, 1, 8 ) ) == "#XDEFINE"
				cn_XDefine()
*			Case Upper ( SubStr ( lsString, 1, 9 ) ) == "#F_DEFINE"
*				cn_Define()
			Case Upper ( SubStr ( lsString, 1, 8 ) ) == "#COMMAND"
				cn_Command()
			Case Upper ( SubStr ( lsString, 1, 9 ) ) == "#XCOMMAND"
				cn_XCommand()
			Case Upper ( SubStr ( lsString, 1, 10 ) ) == "#TRANSLATE"
				cn_Translate()
			Case Upper ( SubStr ( lsString, 1, 11 ) ) == "#XTRANSLATE"
				cn_Translate()
			Case Upper ( SubStr ( lsString, 1, 6 ) ) == "#IFDEF"
				lsString := AllTrim( SubStr ( lsString, 7)) + " "
				ln01ii := At( " ", lsString)
				lsToSearch := SubStr ( lsString, 1, ln01ii - 1)
				Select Define
				Seek &( lsToSearch)
				If lsToSearch == AllTrim ( Define->Content)
					Select Temp
					Delete
					Skip
					cn_Compile ( lnFirst + 1, .T. )
				Else
				EndIf
			Case Upper ( SubStr ( lsString, 1, 7 ) ) == "#IFNDEF"
				lsToSearch := ""
				cn_Compile ( lnFirst + 1)
			Case Upper ( SubStr ( lsString, 1, 6 ) ) == "#ENDIF"
				If lnFirst = 0
					aAdd ( aClipError, { PRINT_FILENAME, Field->FileName, PRINT_LINE, Field->LineNum, "#ENDIF" + PRINT_WITHOUT + "#IF*" })
					cn_Abort()
				EndIf
			Case Upper ( SubStr ( lsString, 1, 5 ) ) == "#ELSE"
				If lnFirst = 0
					aAdd ( aClipError, { PRINT_FILENAME, Field->FileName, PRINT_LINE, Field->LineNum, "#ELSE" + PRINT_WITHOUT + "#IF*" })
					cn_Abort()
				EndIf
			Case Upper ( SubStr ( lsString, 1, 10 ) ) == "#ELSEIFDEF" .or. Upper ( SubStr ( lsString, 1, 8 ) ) == "#ELIFDEF"
				If lnFirst = 0
					aAdd ( aClipError, { PRINT_FILENAME, Field->FileName, PRINT_LINE, Field->LineNum, "#ELSEIFDEF" + PRINT_WITHOUT + "#IF*" })
					cn_Abort()
				EndIf
			Case Upper ( SubStr ( lsString, 1, 7 ) ) == "#ELIFEQ" .or. Upper ( SubStr ( lsString, 1, 9 ) ) == "#ELSEIFEQ"
				If lnFirst = 0
					aAdd ( aClipError, { PRINT_FILENAME, Field->FileName, PRINT_LINE, Field->LineNum, "#ELIFDEF" + PRINT_WITHOUT + "#IF*" })
					cn_Abort()
				EndIf
			Case Upper ( SubStr ( lsString, 1, 8 ) ) == "#ELIFNEQ" .or. Upper ( SubStr ( lsString, 1, 10 ) ) == "#ELSEIFNEQ"
				If lnFirst = 0
					aAdd ( aClipError, { PRINT_FILENAME, Field->FileName, PRINT_LINE, Field->LineNum, "#ELIFNEQ/#ELSEIFNEQ" + PRINT_WITHOUT + "#IF*" })
					cn_Abort()
				EndIf
			Case Upper ( SubStr ( lsString, 1, 11 ) ) == "#ELSEIFNDEF" .or. Upper ( SubStr ( lsString, 1, 9 ) ) == "#ELIFNDEF"
				If lnFirst = 0
					aAdd ( aClipError, { PRINT_FILENAME, Field->FileName, PRINT_LINE, Field->LineNum, "#ELSEIFDEF" + PRINT_WITHOUT + "#IFDEF/#IFNDEF" })
					cn_Abort()
				EndIf
			Case SubStr ( lsString, 1, 1 ) == "*"
			Case SubStr ( lsString, 1, 12 ) == "//"
			Case SubStr ( lsString, 1, 2 ) == "/*"
				lsString := cn_Comment( lsString)
				Temp->Content := lsString
				Loop
			Case Empty ( lsString)
				// pm
			Otherwise
				cn_CompLine(lsString)
			EndCase
			cn_AddOut()
			Select Temp
			Delete
			Skip
		EndDo
		Go Top
	EndDo
	If lnFirst > 0
		Go RecCount()
		aAdd ( aClipError, { PRINT_FILENAME, Field->FileName, PRINT_LINE, Field->LineNum, PRINT_WITHOUT + "#endif" } )
	EndIf