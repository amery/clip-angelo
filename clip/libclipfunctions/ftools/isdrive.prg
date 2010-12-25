function isDrive(c)
	c:=upper(c)
	if !empty(set(c+":"))
		return .t.
	endif
return .f.
