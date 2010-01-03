static function __IF_setValue(sPart,sKey,xData)
	local s1,s2
	s1 := ::checkName(sPart)
	s2 := ::checkName(sKey)
	::__names[s1] := s1
	::__names[s2]  := s2
	if ! (s1 $ ::data)
		::data[s1] := map()
	endif
	::data[s1][s2] := xData
return xData
