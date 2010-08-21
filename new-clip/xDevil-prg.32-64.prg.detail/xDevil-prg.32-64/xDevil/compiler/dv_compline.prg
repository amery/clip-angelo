
 Procedure dv_CompLine ( psString)
	Local laString := {}
	Local ln01ii := 0
	Local lsToSearch
	Select OutPut
	Append Blank
	Field->Content := psString
	Field->FileName := Temp->FileName
	laString := dv_Line2Array( psString )
	Select Define
	For ln01ii := 1 To Len (laString)
		lsToSearch := laString[ ln01ii]
		Seek &(lsToSearch)
		If AllTrim ( Field->Content) == lsToSearch
			laString[ ln01ii] := AllTrim( Field ->Command )
			If Field->cType == "N"
				laString[ ln01ii] := Val( laString[ ln01ii]  )
			EndIf
		EndIf
	Next
	lsToSearch := ""
	For ln01ii := 1 To Len (laString)
		lsToSearch := lsToSearch + " " + laString [ ln01ii]
	Next
	Select OutPut
	OutPut->Command := lsToSearch
	If laString[2] = ":=" .or. laString[2] = "=" .or. laString[2] = "+=" .or. laString[2] = "-=" .or. laString[2] = "*=" .or. laString[2] = "%="
		if p_bDebug
			lsToSearch := Upper(laString[1])
			If aScan(  m->__AParam, lsToSearch) = 0
				aAdd( m->__AParam, { lsToSearch, &(lsToSearch), Temp->FileName, Temp->LineNum, .F., .F. } )
			EndIf
			? "Devil_Debug (", Temp->FileName, Temp->LineNum, "m->__AParam )"
		EndIf
		? AllTrim ( OutPut->Command)
		Return
	EndIf

