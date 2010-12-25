*********************
static function me_setReadOnly(nWin, flag)
local i

	flag := iif(valtype(flag)!="L", .t., flag)
	if valtype(nWin) == "N" .and. between(nWin, 1, ::nWins)
		::tobjinfo[nWin]:readOnly := flag
	endif
	::drawHead()
return
