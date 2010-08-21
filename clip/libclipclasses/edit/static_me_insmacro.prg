************************************
* insert macro
static function me_insMacro(nWin)
local obj := ::__checkWindow(@nWin)
	obj:insMacro(::tempmacro)
return .t.
