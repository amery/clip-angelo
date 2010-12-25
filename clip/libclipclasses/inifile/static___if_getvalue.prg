static function __IF_getValue(sPart,sKey)
	local s1,s2
	s1 := ::checkName(sPart)
	if sKey == NIL
		if s1 $ ::data
			return ::data[s1]
		else
			return map()
		endif
	endif
	s2 := ::checkName(sKey+"_"+host_charset())
	if s1 $ ::data .and. s2 $ ::data[s1]
		return ::data[s1][s2]
	endif
	s2 := ::checkName(sKey)
	if s1 $ ::data .and. s2 $ ::data[s1]
		return ::data[s1][s2]
	endif
return NIL
