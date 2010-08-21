static function me_matchStructure(nWin, lDirect)
local obj := ::__checkWindow(@nWin)
local symb
	lDirect := iif(lDirect == NIL, .t., lDirect)

	symb := obj:curword()
	if !obj:matchStruct(symb, lDirect)
		alert([Structure not found!], "OK")
	endif
return .t.
