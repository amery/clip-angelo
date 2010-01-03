static function te_beginBlock( vid, undo )
    undo := iif(undo==NIL, .t., undo)
    vid := iif(vid==NIL, .t., vid)
    if undo
	::writeundo(HASH_BEGBLOCK)
    endif
    afill(::koordblock, NIL)
    ::cancelBlock()
    ::mkblock := .t.
    if vid
	::strblock := .t.
    else
	::rectblock := .t.
    endif
    ::koordblock[1] := ::line
    ::koordblock[2] := ::pos
    ::koordblock[3] := ::line
    ::koordblock[4] := ::pos
    if undo
	::refresh()
    endif
RETURN
