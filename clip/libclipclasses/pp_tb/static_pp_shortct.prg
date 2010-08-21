static func pp_shortct(nkey)
   local i
   for i=1 to ::itemCount
	if (`SHORTCUT` $ ::__items[i]) .and. ::__items[i]:shortcut==nkey
	    return i
	endif
   next
return 0
