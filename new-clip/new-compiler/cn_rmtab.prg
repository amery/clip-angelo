Function Cn_RmTab ( psString)
	Do While Chr(8) $ psString
		StrTran ( psString, cHr(8), " " )
	EndDo
	Do While Chr(9) $ psString
		StrTran ( psString, Chr(9), " " )
	EndDo
	Return psString

