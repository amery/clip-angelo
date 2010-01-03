static function te_up( undo )
     undo := iif(undo==NIL, .t., undo)
     if undo
	::writeundo(HASH_UP)
     endif

     ::line--
     ::rowWin--
     if undo
	 if ::rowWin < ::nTop+1 .or. ::mkblock
		::refresh()
	 else
		devpos(::nTop+::rowWin-1,::nLeft+::colWin-1)
	 endif
     endif
RETURN
