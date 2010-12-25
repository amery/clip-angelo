 Proc Cn_Include
	Local lsString := ""
	Local lcCollectChar := ""
	Local lsFileName := ""
	Local ln01ii := 0
	Local lnCurRecord := 0
	lsString := AllTrim ( Temp->Content )
	lsString := AllTrim ( SubStr( lsString, 9 ) )
	Do Case
	Case SubStr ( lsString, 1, 1 ) == '"'
		lcCollectChar := '"'
	Case SubStr  ( lsString, 1, 1 ) == "'"
		lcCollectChar := "'"
	Case SubStr  ( lsString, 1, 1 ) == "<"
		lcCollectChar := ">"
	Otherwise
		aAdd ( p_aClipError, { PRINT_FILENAME, Field->FileName, PRINT_LINE, Field->LineNum, ERROR_INCLUDE_STRING } )
	EndCase
	lsFileName := ""
	lsString := SubStr ( sString, 2)
	ln01ii := Len ( lsString )
	Do While ( ln01ii >= 1 ) .and. ( SubStr ( lsString, 2 , 1) # lcCollectChar )
		lsFileName := lsFileName + SubStr ( lsString, 1, 1 )
		lsString := SubStr ( lsString, 2)
	EndDo
	If Empty ( lsString)
		aAdd ( p_aClipError, { PRINT_FILENAME, Field->FileName, PRINT_LINE, Field->LineNum, ERROR_INCLUDE_END })
		cn_Abort()
	EndIf
	lsFileName := lsFileName + SubStr ( sString, 1, 1 )
	lnCurRecord := RecNo()
	cn_ReadFile( lsFileName, RecCount() )
	Go lnCurRecord

