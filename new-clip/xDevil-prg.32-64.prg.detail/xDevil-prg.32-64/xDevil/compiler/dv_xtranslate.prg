Proc dv_XTranslate
	Local lsString := ""
	Local lsResult := ""
	Local lsSource := ""
	Local lsSourceShort := ""
	Local ln01ii := 0
	Select Translate
	lsString := AllTrim ( SubStr ( Temp->Content ))
	lsString := AllTrim ( dv_RmTab ( lsString ) )
	lsString := AllTrim ( SubStr ( lsString, 8 ) )
	ln01ii := At ( "=>", lsString )
	lsSource := AllTrim ( SubStr ( lsString, 1, ln01ii - 1 ) )
	lsString := AllTrim ( SubStr ( lsString, ln01ii + 2 ) ) + " "
	ln01ii := Min ( At ( " ", lsString ), Len ( lsString ) )
	lsResult := Upper ( AllTrim ( SubStr ( lsString, 1, ln01ii ) ) )
	ln01ii := Min ( 4, Len ( lsSource ) )
	lsSourceShort := Upper ( SubStr ( lsSource, 1, ln01ii ) )
	Do While ln01ii <= Len ( lsSource )
		Seek &( lsSourceShort )
		If lsSourceShort == AllTrim( Field->Content)
			dv_Warning ( lsSourceShort, ERROR_DEFINE, PRINT_FILENAME, Field->FileName, PRINT_LINE, Field->LineNum )
		Else
			Append Blank
			Field->Content := lsSourceShort
			Field->Command := lsResult
			Field->LineNum := Temp->LineNum
			Field->FileName := Temp->FileName
		EndIf
		++ln01ii
	EndDo
	Select Temp

