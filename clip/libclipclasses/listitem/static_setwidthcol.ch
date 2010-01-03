static function setWidthCol(width)
local i, az:={}, s:=0, len
	if empty(width) .or. valtype(width)!="A"
		return .f.
	endif
	afill(::lencol, 0)
	for i=1 to ::colWin
		if len(width)<i
			exit
		endif
		::lencol[i] := width[i]
	next
	for i=1 to ::colWin
		if ::lencol[i] == 0
			aadd(az, i)
		else
			s += ::lencol[i]
		endif
	next
	len := (::nRight - ::nLeft - ::colWin+1 - s)/len(az)
	for i=1 to len(az)
		::lencol[az[i]] := len
	next
return .t.
