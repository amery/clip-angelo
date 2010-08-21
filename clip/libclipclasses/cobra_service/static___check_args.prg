static function __check_args(qArgs,nLen,aTypes)
	local i
	if valtype(qArgs) != "A"
		return .f.
	endif
	if len(qArgs) < nLen
		return .f.
	endif
	for i=1 to len(qArgs)
		if len(aTypes) < i
			return .t.
		endif
		if empty(aTypes[i])
			loop
		endif
		if ! (valtype(qArgs[i]) $ aTypes[i])
			return .f.
		endif
	next
return .t.
