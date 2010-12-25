static function me_reloadFile(nWin)
local obj := ::__checkWindow(@nWin)
local k
	if obj:updated
		if alert([There is a change in the file.;]+::tobjinfo[nWin]:name+[; Reload?], {[Yes], [No]}, set("edit_colors_menu")) == 1
			k := obj:loadFile(::tobjinfo[nWin]:name)
		endif
	else
		k := obj:loadFile(::tobjinfo[nWin]:name)
	endif
	if !k
		alert([Can't open file;]+::tobjinfo[nWin]:name, "OK")
	endif
return .t.
