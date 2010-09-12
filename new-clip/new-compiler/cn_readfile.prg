Procedure Cn_ReadFile ( psSourceFileName, pnCurRecord )
	Local ln01ii := 0
	Local ln02ii
	Local lsFileToRead := ""
	Local lbRead := .F.
	Select Temp
	n01ii := 1
	Do While ( ln01ii <= Len ( p_aInclude) ) .and. (.not. lbRead)
		lsFileToRead := p_aInclude [ n01ii ] + DIR_SEPARATOR + psSourceFileName
		If File ( lsFileToRead )
			Append From ( lsFileToRead ) SDF
			lbRead := .T.
			ln02ii := 1
			Go pnCurRecord + 1
			Do While .Not. Eof()
				Field->FileName := SourceFileName
				Field->LineNum := ln02ii
				++ln02ii
				Skip
			EndDo
		EndIf
		++n01ii
	EndDo
	If .not. lbRead
		aAdd( p_aClipError,{ PRINT_FILENAME, pSourceFileName, PRINT_LINE, pnCurRecord, ERROR_FILE } )
		p_bStop := .T.
		cn_Abort()
	EndIf
	Go pnCurRecord
