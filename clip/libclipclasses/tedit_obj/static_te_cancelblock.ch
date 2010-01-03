static function te_cancelBlock( undo )
    undo := iif (undo==NIL, .t., undo)
    ::mkblock := .f.
    ::strblock := .f.
    ::rectblock := .f.
    afill(::koordblock, NIL)
    asize(::__findR, 3)
    afill(::__findR, NIL)
    if undo
	::refresh()
    endif
RETURN
