static func rg_getAccel(nkey)
    local i
    for i=1 to ::itemCount
	if ::__items[i]:isAccel(nkey)
	   return i
	endif
    next
return 0
