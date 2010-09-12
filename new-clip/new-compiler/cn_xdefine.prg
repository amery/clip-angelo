 Proc Cn_XDefine
	Local lsString := ""
	Local lsResult := ""
	Local lsSource := ""
	Local ln01ii := 0
	Select Define
	lsString := AllTrim ( SubStr ( Temp->Content ))
	lsString := AllTrim ( cn_RmTab ( lsString ) )
	lsString := AllTrim ( SubStr ( lsString, 9 ) )
	ln01ii := At ( " ", lsString )
	lsSource := AllTrim ( SubStr ( lsString, 1, ln01ii ) )
	lsString := AllTrim ( SubStr ( lsString, ln01ii ) ) + "  "
	ln01ii := Min ( At ( "  ", lsString ), Len ( lsString ) )
	lsResult := AllTrim ( SubStr ( lsString, 1, ln01ii + 2 ))
	ln011 := Len ( lsResult )
	Do While ( lsResult == lsSource )
		cn_AddDefine ( lsSource, lsResult )
		lsSource := SubStr ( lsSource, 1, ++ln01ii )
	EndDo

