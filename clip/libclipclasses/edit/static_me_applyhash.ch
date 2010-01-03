static function me_applyHash(oMe, nHashCode)
local b, ret
	if !(nHashCode $ oMe:__hashKeys)
		ret := ME_EXCEPTION
		if oMe:tobj[oMe:curwin]:handleKey(nHashCode)
			ret := ME_CONTINUE
		endif
		return ret
	endif

	b := oMe:__hashKeys[nHashCode]
	ret :=  eval(b, oMe, nHashCode)
return ret
