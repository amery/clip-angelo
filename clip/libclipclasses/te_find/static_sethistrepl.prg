static function sethistrepl(hist)
local i
	if valtype(hist) == "A"
		for i:= 1 to len(hist)
			::rhistory:add(hist[i])
		next
	else
		::rhistory:add(hist)
	endif
return
