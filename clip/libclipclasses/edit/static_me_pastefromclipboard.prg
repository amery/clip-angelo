************************************
* paste block from Clipboard
static function me_pasteFromClipboard(nWin)
local obj := ::__checkWindow(@nWin)
	obj:pasteFromClipboard(::Clipboard)
return .t.
