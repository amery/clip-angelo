Procedure cn_AddDefine ( psSource, psResult )
	Select Define
	Seek &( psSource)
	If psSource == AllTrim( Field->Content)
		cn_Warning ( psSource, ERROR_DEFINE, PRINT_FILENAME, Field->FileName, PRINT_LINE, Field->LineNum )
	Else
		Append Blank
		Field->Content := psSource
		Field->Command := psResult
		Field->LineNum := Temp->LineNum
		Field->FileName := Temp->FileName
		If SubStr( psResult, 1, 1 ) $ "0123456789.-+*/"
			Field->cType := "N"
		EndIf
	EndIf
