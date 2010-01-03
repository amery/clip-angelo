static function me_markBlock(nWin, lMode)
local obj := ::__checkWindow(@nWin)
	lMode := iif(lMode==NIL, .t., lMode)
	if obj:mkblock
		obj:endBlock(lMode)    // start block
	else
		obj:beginBlock(lMode)  // stop block
	endif

return .t.
