************************************
* copy block
static function me_copyblock(nWin, lCopy)
local obj := ::__checkWindow(@nWin)
	lcopy := iif(lCopy==NIL, .T., lCopy)
	if lCopy
		obj:copyBlock()
	else
		obj:moveBlock()
	endif
return .t.
