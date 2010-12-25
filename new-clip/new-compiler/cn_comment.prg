 Function cn_Comment( psString )
	Local ln01ii := 0
	Local ln02ii := 0
	Local lnCurRecourd := RecNo()
	Local lsFileName := AllTrim ( Temp->FileName)
	Local lnLine := Temp->LineNum
	Local lsString := ""
	ln01ii := At( "*/", psString)
	If ln01ii > 0
		lsString := SubStr ( psString, 1, ln01ii - 1)
		ln02ii := At( "/*" $ psString )
		psString := SubStr ( psString, ln02ii + 2 )
		psString := lsString + " " + psString
	EndIf
	Do While ln01ii = 0 .and. ( lsFileName == AllTrim ( Temp->FileName ) )
		psString := AllTrim ( Temp->Content ) + "  "
		ln01ii := At( "*/", psString)
		If ln01ii < ( Len( psString) - 1)
			psString := SubStr ( psString, ln01ii + 2 )
			Temp->Content := psString
		Else
			psString := ""
			Delete
			Skip
		EndIf
	EndDo
	If Eof() .or. ( lsFileName != AllTrim ( Temp->FileName ) )
		aAdd ( p_aClipError, PRINT_FILENAME, lsFileName, PRINT_LINE, lnCurRecourd, ERROR_COMMENT)
		cn_Abort()
	EndIf
	Return psString

