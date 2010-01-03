static function me_print(nWin, lprintBlock)
local obj := ::__checkWindow(@nWin)
	lPrintBlock := iif(lPrintBlock==NIL, .f., lPrintBlock)
	if !lPrintBlock
		obj:print()
	elseif !(obj:printBlock())
		alert([Block not marked!], "OK")
	endif

return .t.
