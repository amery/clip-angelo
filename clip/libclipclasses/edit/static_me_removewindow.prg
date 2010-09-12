*********************
static function me_removeWindow(nWin)
   nWin := iif(nWin == NIL, ::curwin, nWin)
   if !between(nWin, 1, ::nWins)
	return
   endif
   adel(::tobj, nWin)
   asize(::tobj, ::nWins-1)
   adel(::tobjinfo, nWin)
   asize(::tobjinfo, ::nWins-1)
   nWin --
   ::nWins --
   if ::nWins > 0 .and. nWin < 1
	nWin := 1
   endif
return
