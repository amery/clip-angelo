static function me_undo(nWin)
local obj := ::__checkWindow(@nWin)
	obj:undo()
return .t.
