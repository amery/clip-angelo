*********************
static function me_getReadOnly(nWin)
	if valtype(nWin) == "N" .and. between(nWin, 1, ::nWins)
		return ::tobjinfo[nWin]:readOnly
	endif
return .f.
