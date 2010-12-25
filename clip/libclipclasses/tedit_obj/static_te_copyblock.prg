static function te_copyBlock(mov, undo)
local i, lenbuf, stbl, endbl, rightbl, leftbl, rowblock, buf, colblock

    undo := iif(undo==NIL, .t., undo)
    if !::strblock .and. !::rectblock
	 return .F.
    endif
    if mov == NIL   // ����������� ����
	mov := .f.
    endif
    if ::strblock
	stbl := min(::koordblock[1], ::koordblock[3])
	endbl := min(max(::koordblock[1], ::koordblock[3]), ::lines)
	rowblock := endbl-stbl+1

	buf := {}
	aeval(::edbuffer, {|item| aadd(buf, item)}, stbl, rowblock)

	if undo
		::writeundo(iif(mov, HASH_MVBL, HASH_CPBL), buf)
	endif
	::line := ::__check_line(::line, .f.)

	if mov
	    for i=stbl to endbl
		adel(::edbuffer, stbl)
	    next
	    //::lines -= rowblock
	    //asize(::edbuffer, ::lines)
	    if ::line > stbl
		::line -= rowblock
	    endif
	else
	   if ::line>::lines
		::lines := rowblock+::line-1
	   else
		::lines += rowblock
	   endif
	   //::lines += rowblock+iif(::line>=::lines, -1, 0)
	   asize(::edbuffer, ::lines)
	endif


	for i=rowblock to 1 step -1
	    ains(::edbuffer, ::line)
	    ::edbuffer[::line] := buf[i]
	next

	::koordblock[1] := ::line
	::koordblock[3] := ::line+rowblock-1
    else
	stbl := min(::koordblock[1], ::koordblock[3])
	endbl := min(max(::koordblock[1], ::koordblock[3]), ::lines)
	::koordblock[1] := stbl
	::koordblock[3] := endbl
	rowblock := endbl-stbl+1

	leftbl := min(::koordblock[2], ::koordblock[4])
	rightbl := max(::koordblock[2], ::koordblock[4])
	::koordblock[2] := leftbl
	::koordblock[4] := rightbl
	colblock := rightbl-leftbl+1

	buf := {}
	for i=stbl to endbl
	    aadd(buf, padr(substr(::edbuffer[i], leftbl, colblock), colblock))
	    if mov
		::edbuffer[i] := substr(::edbuffer[i], 1, leftbl-1);
			       + substr(::edbuffer[i], rightbl+1)
	    endif
	next

	if undo
		::writeundo(iif(mov, HASH_MVBL, HASH_CPBL), buf)
	endif

	lenbuf := len(buf)-1
	::line := ::__check_line(::line+rowblock-1, .f.)
	if ::pos > leftbl .and. mov .and. between(::line, stbl, endbl)
		::pos -= iif( ::pos-leftbl < colblock, -(::pos-leftbl-3), colblock)
	endif
	for i=0 to lenbuf
	     ::__check_line(::line+i, .f.)
	     ::edbuffer[::line+i] := padr(substr(::edbuffer[::line+i], 1, ::pos-1), ::pos-1) ;
				     + buf[i+1];
				     + substr(::edbuffer[::line+i], ::pos)
	next

	::koordblock[1] := ::line
	::koordblock[3] := ::line+rowblock-1
	::koordblock[2] := ::pos
	::koordblock[4] := ::pos+colblock-1
    endif
    ::updated := .t.
    if undo
	::refresh()
    endif
RETURN .T.
