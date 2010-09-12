static function me_deleteEnd(nWin)
* delete string from current pos to end
local obj := ::__checkWindow(@nWin)
	obj:delEnd()
return .t.
