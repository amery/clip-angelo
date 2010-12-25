static function me_deleteLine(nWin)
* delete current line
local obj := ::__checkWindow(@nWin)
	obj:deleteLine()
return .t.
