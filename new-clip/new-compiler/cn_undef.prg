Proc Cn_UnDef
	Local lsString := ""
	Local lsResult := ""
	Local lsSource := ""
	Local ln01ii := 0
	Select Define
	lsString := AllTrim ( SubStr ( Temp->Content ))
	lsString := AllTrim ( cn_RmTab ( lsString ) )
	lsString := AllTrim ( SubStr ( lsString, 7 ) )
	ln01ii := At ( " ", lsString )
	lsSource := AllTrim ( SubStr ( lsString, 1, n01ii ) )
	Select Define
	Seek &( lsSource)
	If lsSource == AllTrim( Field->Content)
		Delete
	EndIf
	Select Temp

