Procedure dv_AddDefine ( psSource, psResult )
	/*
	Adds a #define or #xdefine to the databases
	*/
	Seek &( psSource)
	If psSource == AllTrim( Field->Content)
		dv_Warning ( psSource, ERROR_DEFINE, PRINT_FILENAME, Field->FileName, PRINT_LINE, Field->LineNum )
	Else
		Append Blank
		Field->Content := psSource
		Field->Command := psResult
		Field->LineNum := Temp->LineNum
		Field->FileName := Temp->FileName
		If SubStr( psResult, 1, 1 ) $ "0123456789.-+*/"
			Field->cType := "N"
		Else
			Field->cType := "C"
		EndIf
	EndIf
