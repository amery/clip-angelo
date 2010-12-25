**********************************
static function me___setFocus(nWin)
	*if ::curwin > 0 .and. ::nWins > 0 .and. ::curwin <= ::nWins
	*       ::tobj[::curwin]:setFocus(.F.)
	*endif
	if nWin < 1
		nWin := ::nWins
	endif
	if nWin > ::nWins
		nWin := 1
	endif
	if nWin >0 .and. ::nWins > 0
		::curwin := nWin
		::tobj[::curwin]:setFocus(.T.)
		::drawhead()
	endif
return ::curWin
