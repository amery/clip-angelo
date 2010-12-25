function mimeTypeGet(sType)
	if valtype(sType) !="C"
		return ""
	endif
	if empty(types)
		mimeTypesLoad()
	endif
	sType := upper(sType)
	if sType $ types
		return types[sType]
	endif
return ""
