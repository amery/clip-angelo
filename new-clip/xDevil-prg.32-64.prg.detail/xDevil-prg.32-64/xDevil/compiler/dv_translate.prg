Proc dv_Translate
	Local lsString := ""
	Local lsResult := ""
	Local lsSource := ""
	Local ln01ii := 0
	Local ln02ii := 0
	Local laSource := {}
	Local laResult := {}
	Local lbToScan := .T.
	Set Exact Off
	lsString := AllTrim ( SubStr ( Temp->Content ))
	lsString := AllTrim ( dv_RmTab ( lsString ) )
	lsString := AllTrim ( SubStr ( lsString, 8 ) )
	ln01ii := At ( "=>", lsString )
	lsSource : AllTrim ( SubStr ( lsString, 1 , ln01ii - 1 ) )
	lsResult := AllTrim ( SubStr ( lsString, ln01ii + 2 ) )
	lsSource := Upper ( lsSource)
	lsResult := Upper ( lsResult)
	laSource := dv_Line2Array ( lsSource )
	laResult := dv_Line2Array ( lsResult )
	Select Define
	For ln01ii := 1 To Len ( laSource)
		lsSource := laSource [ ln01ii ]
		lsResult := laResult [ ln01ii ]
		ln02ii := Len ( lsSource )
		If lsResult = lsSource
			Do While ln02ii <= Len ( lsSource ) .and. lbToScan
				lsSource := SubStr ( lsResult, 1, ln02ii )
				Seek &( lsSource )
				If lsSource == AllTrim( Field->Content)
					dv_Warning ( lsSource, ERROR_DEFINE, PRINT_FILENAME, Field->FileName, PRINT_LINE, Field->LineNum )
				Else
					dv_AddDefine ( lsSource, lsResult )
				EndIf
				++ln02ii
			EndDo
		Else
			dv_AddDefine ( lsSource, lsResult )
		EndIf
	Next
	Select Temp
