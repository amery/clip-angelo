Static Procedure dv_ReadRC
	Local laDbStruct := {}
	Local laParameters := {}
	Local lsString := ""
	Local lsString2 := ""
	Local ln01ii := 0
	p_cHomeDir := GetEnv ( "HOME" )
	aAdd ( laDbStruct, { "Line", "C", 2048, 0} )
	DbCreate( (p_cHomeDir + "/tmp/temp"), laDbStruct )
	Use (p_cHomeDir + "/tmp/temp") exclusive
	Set Delete On
	if File ( p_cHomeDir + "/.xDevilrc" )
		Append from ( p_cHomeDir + "/.xDevilrc" ) SDF
	EndIf
	if File ( "./.xDevilrc" )
		Append from "./.xDevilrc" SDF
	EndIf
	if File ( xDevil_ROOT + "./etc/environment" )
		Append from (xDevil_ROOT +  "./etc/environment") SDF
	EndIf
	if File ( xDevil_ROOT + "./lang/" + vexDevil_LANG )
		Append from (xDevil_ROOT + "./lang/" + vexDevil_LANG ) SDF
	EndIf
	if File ( xDevil_ROOT + "./lang/" + vexDevil_LC_ALL )
		Append from (xDevil_ROOT + "./lang/" + vexDevil_LC_ALL) SDF
	End
	if File ( xDevil_ROOT + "./lang/" + vexDevil_CHARSET )
		Append from (xDevil_ROOT + "./lang/" + vexDevil_CHARSET)
	EndIf
	if File ( xDevil_ROOT + "./term/" + vexDevil_TERM )
		Append from ("./term/" + vexDevil_TERM) SDF
	EndIf
	Go Top
	Do While .not. Eof()
		lsString := AllTrim ( Temp->line)
		Do While Len( lsString ) > 0
			If SubStr( lsString, 1, 1 ) = "#"
				lsString := ""
			ElseIf ( " " $ lsString )
					ln01ii := At(" ", lsString )
					lsString2 := SubStr ( lsString, 1, ln01ii)
					AAdd ( laParameters, AllTrim ( lsString2 ) )
					lsString := AllTrim ( SubStr ( lsString, ln01ii ) )
			Else
				AAdd ( laParameters, lsString)
				lsString := ""
			EndIf
		EndDo
		Delete
		Go Top
	EndDo
	Use
	fErase( p_cHomeDir + "/tmp/temp.dbf" )
	Return laParameters

