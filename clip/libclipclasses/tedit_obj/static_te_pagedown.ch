static function te_PageDown( undo )
    undo := iif(undo==NIL, .t., undo)
    if undo
	::writeundo(HASH_PGDOWN)
    endif
    ::line := min(::line+::nbottom-::ntop, ::lines)
    if undo
	::refresh()
    endif
return
