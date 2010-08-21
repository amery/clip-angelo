

Procedure dv_Stdch ( pnCurRecord )
	Local ln01ii := 0
	Local ln02ii := 0
	Local ln03ii := 0
	Local lsWord := ""
	Local lsResult := ""
	Select Define
	If File ( xDevil_ROOT + "/db/define.dbf" )
		Append From ( xDevil_ROOT + "/db/define.dbf" )
	Else
			pnCurRecord := dv_ReadFile ( "dv_new_std.ch", RecNo() + 1 )
	EndIf
	Select Command
	If File ( xDevil_ROOT + "/db/command.dbf" )
		Append From ( xDevil_ROOT + "/db/command.dbf" )
	Else
			pnCurRecord := dv_ReadFile ( "dv_new_std.ch", RecNo() + 1 )
	EndIf

