/*
    dv_compile.prg

*/
Procedure dv_Compile ( lnFirst, pbCollect )
	Static saDefine := {}
	Local ln01ii := 0
	Local ln02ii := 0
	Local lnCurRecord := 0
	Local lsString := ""
	Local lsToSearch := ""
	Default pbCollect To .F.
	Do While .not. Eof()
		Do While .not. Eof()
			lsString := AllTrim( dv_RmTab ( Field->Content ) )
			Do While rAt( lsString, 1 ) == ";"
				Delete
				lsString := SubStr( lsString, 1, Len(lsString) - 1 )
				Skip
				lsString := AllTrim ( lsString ) + " " + AllTrim ( dv_RmTab ( AllTrim ( Field->Content ) ) )
			EndDo
			Field->Content := lsString
			Do Case
			Case Upper ( SubStr ( lsString, 1, 8 ) ) == "#INCLUDE"
				dv_Include ()
			Case Upper ( SubStr ( lsString, 1, 7 ) ) == "#DEFINE"
				dv_Define()
			Case Upper ( SubStr ( lsString, 1, 6 ) ) == "#UNDEF"
				dv_UnDef()
			Case Upper ( SubStr ( lsString, 1, 8 ) ) == "#XDEFINE"
				dv_XDefine()
*			Case Upper ( SubStr ( lsString, 1, 9 ) ) == "#F_DEFINE"
*				dv_Define()
			Case Upper ( SubStr ( lsString, 1, 8 ) ) == "#COMMAND"
				dv_Command()
			Case Upper ( SubStr ( lsString, 1, 9 ) ) == "#XCOMMAND"
				dv_XCommand()
			Case Upper ( SubStr ( lsString, 1, 10 ) ) == "#TRANSLATE"
				dv_Translate()
			Case Upper ( SubStr ( lsString, 1, 11 ) ) == "#XTRANSLATE"
				dv_Translate()
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
					dv_Compile ( lnFirst + 1, .T. )
				Else
				EndIf
			Case Upper ( SubStr ( lsString, 1, 7 ) ) == "#IFNDEF"
				lsToSearch := ""
				dv_Compile ( lnFirst + 1)
			Case Upper ( SubStr ( lsString, 1, 6 ) ) == "#ENDIF"
				If lnFirst = 0
					aAdd ( p_aDevilError, { PRINT_FILENAME, Field->FileName, PRINT_LINE, Field->LineNum, "#ENDIF" + PRINT_WITHOUT + "#IF*" })
					dv_Abort()
				EndIf
			Case Upper ( SubStr ( lsString, 1, 5 ) ) == "#ELSE"
				If lnFirst = 0
					aAdd ( p_aDevilError, { PRINT_FILENAME, Field->FileName, PRINT_LINE, Field->LineNum, "#ELSE" + PRINT_WITHOUT + "#IF*" })
					dv_Abort()
				EndIf
			Case Upper ( SubStr ( lsString, 1, 10 ) ) == "#ELSEIFDEF" .or. Upper ( SubStr ( lsString, 1, 8 ) ) == "#ELIFDEF"
				If lnFirst = 0
					aAdd ( p_aDevilError, { PRINT_FILENAME, Field->FileName, PRINT_LINE, Field->LineNum, "#ELSEIFDEF" + PRINT_WITHOUT + "#IF*" })
					dv_Abort()
				EndIf
			Case Upper ( SubStr ( lsString, 1, 7 ) ) == "#ELIFEQ" .or. Upper ( SubStr ( lsString, 1, 9 ) ) == "#ELSEIFEQ"
				If lnFirst = 0
					aAdd ( p_aDevilError, { PRINT_FILENAME, Field->FileName, PRINT_LINE, Field->LineNum, "#ELIFDEF" + PRINT_WITHOUT + "#IF*" })
					dv_Abort()
				EndIf
			Case Upper ( SubStr ( lsString, 1, 8 ) ) == "#ELIFNEQ" .or. Upper ( SubStr ( lsString, 1, 10 ) ) == "#ELSEIFNEQ"
				If lnFirst = 0
					aAdd ( p_aDevilError, { PRINT_FILENAME, Field->FileName, PRINT_LINE, Field->LineNum, "#ELIFNEQ/#ELSEIFNEQ" + PRINT_WITHOUT + "#IF*" })
					dv_Abort()
				EndIf
			Case Upper ( SubStr ( lsString, 1, 11 ) ) == "#ELSEIFNDEF" .or. Upper ( SubStr ( lsString, 1, 9 ) ) == "#ELIFNDEF"
				If lnFirst = 0
					aAdd ( p_aDevilError, { PRINT_FILENAME, Field->FileName, PRINT_LINE, Field->LineNum, "#ELSEIFDEF" + PRINT_WITHOUT + "#IFDEF/#IFNDEF" })
					dv_Abort()
				EndIf
			Case SubStr ( lsString, 1, 1 ) == "*"
			Case SubStr ( lsString, 1, 12 ) == "//"
			Case SubStr ( lsString, 1, 2 ) == "/*"
				lsString := dv_Comment( lsString)
				Temp->Content := lsString
				Loop
			Case Empty ( lsString)
				// pm
			Otherwise
				dv_CompLine(lsString)
			EndCase
			dv_AddOut()
			Select Temp
			Delete
			Skip
		EndDo
		Go Top
	EndDo
	If lnFirst > 0
		Go RecCount()
		aAdd ( p_aDevilError, { PRINT_FILENAME, Field->FileName, PRINT_LINE, Field->LineNum, PRINT_WITHOUT + "#endif" } )
	EndIf