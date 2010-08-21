************************************
static function me_savePos(nWin)
local obj := ::__checkWindow(@nWin)
	::__Hist:insert(alltrim(toString(obj:line)))
return .t.
