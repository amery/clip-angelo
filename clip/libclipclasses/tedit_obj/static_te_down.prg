static function te_down( undo )
    undo := iif(undo==NIL, .t., undo)
    if undo
	::writeundo(HASH_DOWN)
    endif

    ::line++
    ::rowWin++
    if undo
	if (::rowWin > (::nBottom-::nTop+1)) .or. ::mkblock
		::refresh()
	else
		devpos(::nTop+::rowWin-1,::nLeft+::colWin-1)
	endif
    endif
RETURN
