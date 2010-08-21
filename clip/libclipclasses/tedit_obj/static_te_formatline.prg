static function te_formatLine(lAutoMargin, nMarginLeft, nMarginRight, nTabSize, lHyphen, undo)
local s1, s2, firstPos
	undo := iif(undo==NIL, .t., undo)

	lAutoMargin := iif(lAutoMargin==NIL, .t., lAutoMargin)
	nMarginLeft := iif(nMarginLeft==NIL, ::MarginLeft, nMarginLeft)
	nMarginRight := iif(nMarginRight==NIL, ::MarginRight, nMarginRight)
	nTabSize := iif(nTabSize==NIL, ::TabSize, nTabsize)
	lHyphen := iif(lHyphen==NIL, ::Hyphen, lHyphen)

	::__check_line(::line+1)
	if undo
		::writeundo(HASH_FRLINE, ::edbuffer[::line])
	endif

	if lAutoMargin
		firstPos := nMarginLeft
	else
		firstPos := len(::edbuffer[::line])-len(ltrim(::edbuffer[::line]))
		firstPos := iif(firstPos == 1, nTabSize, firstPos)
	endif

	::edbuffer[::line] := stripSpace(::edbuffer[::line], 1)
	::edbuffer[::line] := replicate(" ", FirstPos) + ltrim(::edbuffer[::line])
	::__Frmt(::edbuffer[::line], nMarginRight, @s1, @s2, lHyphen, firstPos)

	::lines += 1
	asize(::edbuffer, ::lines)
	ains(::edbuffer, ::line+1)
	::edbuffer[::line] := s1
	::edbuffer[::line+1] := s2

	::updated := .t.
	if undo
		::refresh()
	endif
RETURN
