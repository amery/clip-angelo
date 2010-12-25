static function te_newLine(undo, autoIndent)
    undo := iif(undo==NIL, .t., undo)
    autoIndent := iif(autoIndent==NIL, .t., autoIndent)
    if undo
	::writeundo(HASH_NEWLINE)
    endif
    if ::line+1<=::lines
	::line++
	::rowWin++
	if autoIndent
		::pos := iif(::line-1 >= 1, len(::edbuffer[::line-1])-len(ltrim(::edbuffer[::line-1]))+1, 1)
	else
		::pos := 1
	endif
	::colWin := ::marginLeft
    else
	::pos := 1
    endif
    if undo
	::refresh()
    endif
return
