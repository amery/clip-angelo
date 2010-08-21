Procedure dv_ReadFile ( psSourceFileName, pnCurRecord )
	Local ln01ii := 0
	Local ln02ii
	Local lsFileToRead := ""
	Local lbRead := .F.
	Select Temp
	ln01ii := 1
	Do While ( ln01ii <= Len ( p_aInclude) ) .and. (.not. lbRead)
		lsFileToRead := p_aInclude [ ln01ii ] + DIR_SEPARATOR + psSourceFileName
		If File ( lsFileToRead )
			Append From ( lsFileToRead ) SDF
			lbRead := .T.
			ln02ii := 1
			Go pnCurRecord + 1
			Do While .Not. Eof()
				Field->FileName := psSourceFileName
				Field->LineNum := ln02ii
				++ln02ii
				Skip
			EndDo
		EndIf
		++ln01ii
	EndDo
	If .not. lbRead
		aAdd( p_aDevilError,{ PRINT_FILENAME, psSourceFileName, PRINT_LINE, pnCurRecord, ERROR_FILE } )
		p_bStop := .T.
		dv_Abort()
	EndIf
	Go pnCurRecord
