static function me_newLine(nWin)
local obj := ::__checkWindow(@nWin)
	if ( Set(_SET_INSERT) )
		obj:insertLine(, ::autoIndent)
	else
		obj:newLine(, ::autoIndent)
	endif
return .t.
