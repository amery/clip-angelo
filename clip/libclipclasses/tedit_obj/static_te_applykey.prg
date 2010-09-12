static function te_applyKey(oTe, nKey)
local ret := TE_EXCEPTION, m
	m := oTe:__keys
	if nKey $ m
		ret := eval(m[nKey], oTe, nKey)
	endif
return ret
