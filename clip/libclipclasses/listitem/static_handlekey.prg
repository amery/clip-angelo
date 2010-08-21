static function handleKey(oL, nKey)
local ret := LI_CONTINUE
	if !(nKey $ oL:__keys)
		return ret
	endif

	ret := eval(oL:__keys[nKey], oL, nKey)
return ret
