static function me_setHash(hKey, vData)
local ret
	ret := vData
	if hKey $ ::__hashKeys
		ret := ::__hashKeys[hKey]
	endif
	::__hashKeys[hKey] := vData
	if vData == NIL
		adel(::__hashKeys, hKey)
	endif
return ret
