 Proc dv_Define
	Local lsString := ""
	Local lsResult := ""
	Local lsSource := ""
	Local ln01ii := 0
	Select Define
	lsString := AllTrim ( SubStr ( Temp->Content ))
	lsString := AllTrim ( dv_RmTab ( lsString ) )
	lsString := AllTrim ( SubStr ( lsString, 8 ) )
	ln01ii := At ( " ", lsString )
	lsSource := AllTrim ( SubStr ( lsString, 1, ln01ii ) )
	lsString := AllTrim ( SubStr ( lsString, ln01ii ) ) + " "
	ln01ii := Min ( At ( " ", lsString ), Len ( lsString ) )
	lsResult := AllTrim ( SubStr ( lsString, 1, ln01ii ))
	Seek &( lsSource)
	If lsSource == AllTrim( Field->Content)
		dv_Warning ( lsSource, ERROR_DEFINE, PRINT_FILENAME, Field->FileName, PRINT_LINE, Field->LineNum )
	Else
		dv_AddDefine ( lsSource, lsResult )
	EndIf
	Select Temp

