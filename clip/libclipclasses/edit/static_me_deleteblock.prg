************************************
* remove block
static function me_deleteBlock(nWin)
local obj := ::__checkWindow(@nWin)
	obj:deleteBlock()
return .t.
