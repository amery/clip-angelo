function restsetkey(savedkeys)
	local i
	if savedKeys==NIL
		return .f.
	endif
	for i=MIN_KEY_CODE to MAX_KEY_CODE
		setkey(i,savedKeys[i])
	next
return .t.
