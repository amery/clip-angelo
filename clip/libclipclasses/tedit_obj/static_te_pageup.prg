static function te_PageUp( undo )
    undo := iif(undo==NIL, .t., undo)
    if undo
	::writeundo(HASH_PGUP)
    endif
    ::line=::line-::nbottom-::ntop
    if undo
	::refresh()
    endif
RETURN
