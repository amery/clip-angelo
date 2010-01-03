static function me_deleteHome(nWin)
* delete string from current pos to begin
local obj := ::__checkWindow(@nWin)
	obj:delHome()
return .t.
