FUNCTION GetFldVar(nField)
	LOCAL name := -1
	local oGet := oStatus:activeget
	if valType(nField) == "N"
		if nField > 0  .and. nField <= len(aGetList)
			oGet := agetList[nField]
		else
			return ( name )
		endif
	endif
	if valtype(oGet) == "O"
		name := oGet:name
	endif
RETURN ( name )
