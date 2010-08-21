static function te_formatPart(lAutoMargin, nMarginLeft, nMarginRight, nTabSize, lHyphen, undo)
local i, s1, s2, line, st, en, arr:={}, firstPos1, firstPos2, len

	undo := iif(undo==NIL, .t., undo)

	lAutoMargin := iif(lAutoMargin==NIL, .t., lAutoMargin)
	nMarginLeft := iif(nMarginLeft==NIL, ::MarginLeft, nMarginLeft)
	nMarginRight := iif(nMarginRight==NIL, ::MarginRight, nMarginRight)
	nTabSize := iif(nTabSize==NIL, ::TabSize, nTabsize)
	lHyphen := iif(lHyphen==NIL, ::Hyphen, lHyphen)

	line := ::line
	::__check_line(line+1)
	len := nMarginRight     //length string
	/* start part */
	st := ::line

	firstPos1 := len(::edbuffer[::line])-len(ltrim(::edbuffer[::line]))
	firstPos2 := len(::edbuffer[::line+1])-len(ltrim(::edbuffer[::line+1]))

	/* end part */
	en := ::line
	while en+1 <= ::lines
		if firstPos2 != len(::edbuffer[en+1])-len(ltrim(::edbuffer[en+1]))
			exit
		endif
		aadd(arr, ::edbuffer[en])
		::edbuffer[line] += " " + ::edbuffer[en+1]
		en ++
	enddo
	aadd(arr, ::edbuffer[en])

	if undo
		::writeundo(HASH_FRPART, arr)
	endif

	if lAutoMargin
		firstPos1 := nMarginLeft
		firstPos2 := 0
	endif

	line := st
	::edbuffer[line] := stripSpace(::edbuffer[line], firstPos1)
	while line <= en
		s1 := ""
		s2 := ""
		if line == st
			::edbuffer[line] := replicate(" ", firstPos1) + ltrim(::edbuffer[line])
			::__Frmt(::edbuffer[line], nMArginRight, @s1, @s2, lHyphen, firstPos1)
		else
			::edbuffer[line] := replicate(" ", firstPos2) + ltrim(::edbuffer[line])
			::__Frmt(::edbuffer[line], nMarginRight, @s1, @s2, lHyphen, firstPos2)
		endif
		::__Frmt(::edbuffer[line], nMarginRight, @s1, @s2, lHyphen)
		::__check_line(line+1)
		::edbuffer[line] := s1
		if line == en
			::lines ++
			asize(::edbuffer, ::lines)
			ains(::edbuffer, line+1)
			::edbuffer[line+1] := replicate(" ", firstPos2) + s2
		else
			::edbuffer[line+1] := ltrim(s2)
		endif
		line ++
		if (line > en) .and. (len(::edbuffer[line]) > len)
			en ++
		endif
	enddo

	::pos := 1
	::line := line
	::updated := .t.
	if undo
		::refresh()
	endif
RETURN
