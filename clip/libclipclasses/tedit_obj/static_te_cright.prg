static function te_cright( undo )
    undo := iif(undo==NIL, .t., undo)
    if undo
	::writeundo(HASH_CRIGHT)
    endif
    ::pos++
    ::colWin++
    if undo
	if ::colWin > (::nRight-::nLeft+1) .or. ::mkblock
		::refresh()
	else
		devpos(::nTop+::rowWin-1,::nLeft+::colWin-1)
	endif
    endif
return
