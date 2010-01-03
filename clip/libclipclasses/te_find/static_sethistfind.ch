static function sethistfind(hist)
local i
	if valtype(hist) == "A"
		for i:= 1 to len(hist)
			::fhistory:add(hist[i])
		next
	else
		::fhistory:add(hist)
	endif
return
