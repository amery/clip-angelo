************************************
* insert template
static function me_insTemplate(nWin)
local obj := ::__checkWindow(@nWin)
	obj:insTempl(::templ)
return .t.
