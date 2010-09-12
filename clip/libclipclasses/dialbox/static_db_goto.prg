static function db_goto(line, pos, row, col)
	if (line==NIL .or. line < 1 .or. line > ::lines) .and. (pos==NIL .or. pos<1)
		return .f.
	endif
	::killFocus()

	if !empty(line)
		::line := line
		::rowWin := iif(row==NIL,min((::nBottom-::nTop+1)*2/3, ::line), row)
	endif
	if !empty(pos)
		::pos := pos
		::ColWin := iif(col==NIL, min((::nRight-::nLeft+1)*2/3, ::pos), col)
	endif
	::refresh()
return .t.
