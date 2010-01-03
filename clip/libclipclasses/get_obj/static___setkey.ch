static function __setkey(nKey,value)
	local ret,m := ::__keys

	if nkey $ m
		ret := m[nKey]
	endif
	if pcount() > 2   /* self as first parameter */
		m[nKey] := value
	endif
	if nKey $ m .and. m[nKey] == NIL
		adel(m ,nKey)
	endif
return ret
