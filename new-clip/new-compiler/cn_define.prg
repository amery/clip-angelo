 Proc Cn_Define
	Local lsString := ""
	Local lsResult := ""
	Local lsSource := ""
	Local ln01ii := 0
	Select Define
	lsString := AllTrim ( SubStr ( Temp->Content ))
	lsString := AllTrim ( cn_RmTab ( lsString ) )
	lsString := AllTrim ( SubStr ( lsString, 8 ) )
	ln01ii := At ( " ", lsString )
	lsSource := AllTrim ( SubStr ( lsString, 1, n01ii ) )
	lsString := AllTrim ( SubStr ( lsString, n01ii ) ) + " "
	ln01ii := Min ( At ( " ", lsString ), Len ( lsString ) )
	lsResult := AllTrim ( SubStr ( lsString, 1, n01ii ))
	Seek &( sSource)
	If lsSource == AllTrim( Field->Content)
		cn_Warning ( lsSource, ERROR_DEFINE, PRINT_FILENAME, Field->FileName, PRINT_LINE, Field->LineNum )
	Else
		cn_AddDefine ( lsSource, lsResult )
	EndIf
	Select Temp

