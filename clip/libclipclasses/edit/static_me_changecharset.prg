************************************
static function me_changeCharset(nWin, charSet)
local pathCP:=cliproot()+PATH_DELIM+"charsets"
	if nWin==NIL .or. !between(nWin, 1, ::nWins)
		return .f.
	endif
	if charSet == NIL .or. !file(pathCP+PATH_DELIM+charSet)
		return .f.
	endif
	::tobj[nWin]:setCharset(charSet)
return .t.
