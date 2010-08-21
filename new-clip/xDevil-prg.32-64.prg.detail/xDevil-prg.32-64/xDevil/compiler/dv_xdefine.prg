 Proc dv_XDefine
	Local lsString := ""
	Local lsResult := ""
	Local lsSource := ""
	Local ln01ii := 0
	Select xDefine
	lsString := AllTrim ( SubStr ( Temp->Content ))
	lsString := AllTrim ( dv_RmTab ( lsString ) )
	lsString := AllTrim ( SubStr ( lsString, 9 ) ) + " "
	ln01ii := At ( " ", lsString )
	lsSource := Upper( AllTrim ( SubStr ( lsString, 1, ln01ii ) ) )
	lsString := AllTrim ( SubStr ( lsString, ln01ii ) ) + " "
	ln01ii := Min ( At ( " ", lsString ), Len ( lsString ) )
	lsResult := AllTrim ( SubStr ( lsString, 1, ln01ii ))
	ln01ii := Len ( lsResult )
	dv_AddDefine ( lsSource, lsResult )

