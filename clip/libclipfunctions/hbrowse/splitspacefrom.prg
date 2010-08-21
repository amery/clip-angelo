function splitSpaceFrom(str, spos)
local ret:=0
	str := substr(str, 1, spos)
	ret := atr(" ", str)
	if ret == 0
		ret := spos
	endif
return ret
