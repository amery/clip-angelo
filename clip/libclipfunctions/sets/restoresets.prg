function restoreSets(m)
	local i
	if valtype(m)!="O"
		return .f.
	endif

	for i in m KEYS
	      set(i,m[i])
	next
return .t.
