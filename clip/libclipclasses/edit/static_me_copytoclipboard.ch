static function me_copyToClipboard(nWin)
local obj := ::__checkWindow(@nWin)
	obj:copyToClipboard(::Clipboard)
return .t.
