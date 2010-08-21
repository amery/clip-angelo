static function me_matchSymbol(nWin, lDirect)
local obj := ::__checkWindow(@nWin)
local symb
	lDirect := iif(lDirect == NIL, .t., lDirect)

	if !obj:Identity(@symb, lDirect)
		alert([Symbol ']+symb+[' not found!], "OK")
	endif
return .t.
