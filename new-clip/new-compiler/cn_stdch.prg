

Procedure cn_Stdch ( nCurRecord )
	Local ln01ii := 0
	Local ln02ii := 0
	Local ln03ii := 0
	Local lsWord := ""
	Local lsResult := ""
	Select Define
	If File ( CLIP_ROOT + "/db/define.dbf" )
		Append From ( CLIP_ROOT + "/db/define.dbf" )
	Else
			lnCurRecord := cn_ReadFile ( "cn_new_std.ch", RecNo() + 1 )
	EndIf
	Select Command
	If File ( CLIP_ROOT + "/db/command.dbf" )
		Append From ( CLIP_ROOT + "/db/command.dbf" )
	Else
			lnCurRecord := cn_ReadFile ( "cn_new_std.ch", RecNo() + 1 )
	EndIf

