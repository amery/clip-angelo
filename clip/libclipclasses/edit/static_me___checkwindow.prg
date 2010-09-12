static function me___checkWindow(nWin)
	if nWin == NIL .or. valtype(nWin) != "N"
		nWin := ::curwin
	endif
	if !between(nWin, 1, ::nWins)
		nWin := ::curwin
	endif

return ::tobj[nWin]
