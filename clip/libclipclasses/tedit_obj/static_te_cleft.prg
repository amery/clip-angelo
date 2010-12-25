static function te_cleft( undo )
    undo := iif(undo==NIL, .t., undo)
    if undo
	::writeundo(HASH_CLEFT)
    endif
    ::pos--
    ::colWin--
    if undo
	if ::colWin <= 1 .or. ::mkblock
		::refresh()
	else
		devpos(::nTop+::rowWin-1,::nLeft+::colWin-1)
	endif
    endif
return
