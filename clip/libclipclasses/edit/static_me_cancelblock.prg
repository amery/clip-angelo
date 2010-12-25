static function me_cancelBlock(nWin)
local obj := ::__checkWindow(@nWin)
	obj:cancelBlock()
return .t.
