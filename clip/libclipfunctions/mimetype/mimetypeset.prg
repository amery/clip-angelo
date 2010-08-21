function mimeTypeSet(sType,sData)
	if valtype(sType) !="C"
		return .f.
	endif
	if valtype(sData) !="C"
		return .f.
	endif
	sType := upper(sType)
	if empty(types)
		mimeTypesLoad()
	endif
	types[sType] := sData
return .t.
