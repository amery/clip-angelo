Static Procedure cn_ReadRC
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
	if File ( p_cHomeDir + "/.cliprc" )
		Append from ( p_cHomeDir + "/.cliprc" ) SDF
	EndIf
	if File ( "./.cliprc" )
		Append from "./.cliprc" SDF
	EndIf
	if File ( CLIP_ROOT + "./etc/environment" )
		Append from (CLIP_ROOT +  "./etc/environment") SDF
	EndIf
	if File ( CLIP_ROOT + "./lang/" + veCLIP_LANG )
		Append from (CLIP_ROOT + "./lang/" + veCLIP_LANG ) SDF
	EndIf
	if File ( CLIP_ROOT + "./lang/" + veCLIP_LC_ALL )
		Append from (CLIP_ROOT + "./lang/" + veCLIP_LC_ALL) SDF
	End
	if File ( CLIP_ROOT + "./lang/" + veCLIP_CHARSET )
		Append from (CLIP_ROOT + "./lang/" + veCLIP_CHARSET)
	EndIf
	if File ( CLIP_ROOT + "./term/" + veCLIP_TERM )
		Append from ("./term/" + veCLIP_TERM) SDF
	EndIf
	Go Top
	Do While .not. Eof()
		lsString := AllTrim ( Temp->line)
		Do While Len( lsString ) > 0
			If SubStr( lsString, 1, 1 ) = "#"
				lsString := ""
			ElseIf ( " " $ lsString )
					ln01ii := At(" ", lsString )
					lsString2 := SubStr ( lsString, 1, n01ii)
					AAdd ( laParameters, AllTrim ( lsString2 ) )
					cString := AllTrim ( SubStr ( lsString, n01ii ) )
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
	Return aParameters

