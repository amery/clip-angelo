static function first()
local ret := ""
	if len(::history) == 0
		return ret
	endif
	if ::history[1] != NIL
		ret := ::history[1]
	endif
return ret
