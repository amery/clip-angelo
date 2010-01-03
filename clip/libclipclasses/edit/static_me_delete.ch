static function me_delete(nWin)
* delete current symbol
local obj := ::__checkWindow(@nWin)
	obj:delRight()
return .t.
