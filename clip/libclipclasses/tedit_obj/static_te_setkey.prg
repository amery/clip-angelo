static function te_setKey(nKey, vData)
local ret
	ret := vData
	if nKey $ ::__keys
		ret := ::__keys[nKey]
	endif
	::__keys[nKey] := vData
	if vData == NIL
		adel(::__keys, nKey)
	endif
return ret
