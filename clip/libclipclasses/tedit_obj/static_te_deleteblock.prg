static function te_deleteBlock(undo)
local rowblock, rightbl, leftbl, stbl, endbl, i, unarr:={}, arr
    undo := iif(undo==NIL, .t., undo)
    if !::strblock .and. !::rectblock
	 return .f.
    endif
    if ::strblock
	stbl := min(::koordblock[1], ::koordblock[3])
	endbl := max(::koordblock[1], ::koordblock[3])
	if endbl>::lines
		endbl := ::lines
	endif
	rowblock := endbl-stbl+1
	if undo
		/*
		for i=stbl to endbl
			aadd(unarr, ::edbuffer[i])
		next
		*/
		aeval(::edbuffer, {|item| aadd(unarr, item)}, stbl, rowblock)
		::writeundo(HASH_DELBL, unarr)
	endif

	arr := ::edbuffer
	i := stbl
	aeval(::edbuffer, {|item| arr[i]:=item, i:=i+1}, endbl+1)
	::edbuffer := arr
	/*
	for i=stbl to endbl
	    adel(::edbuffer, stbl)
	next
	*/

	::lines -= rowblock
	asize(::edbuffer, ::lines)
	if ::line-::rowWin > stbl
		::rowWin := 1
	else
		::rowWin -= ::line-stbl
	endif
	::line := stbl
	::pos := 1
    else
	stbl := min(::koordblock[1], ::koordblock[3])
	endbl := max(::koordblock[1], ::koordblock[3])
	leftbl := min(::koordblock[2], ::koordblock[4])
	rightbl := max(::koordblock[2], ::koordblock[4])
	if endbl>::lines
		endbl := ::lines
	endif
	if undo
		aeval(::edbuffer, {|item| aadd(unarr, substr(item, leftbl, rightbl-leftbl+1))}, stbl, endbl-stbl+1)
		::writeundo(HASH_DELBL, unarr)
	endif

	aeval(::edbuffer, {|item| item:=substr(item,1,leftbl-1)+substr(item, rightbl+1)}, stbl, endbl-stbl+1)

	if ::line-::rowWin > stbl
		::rowWin := 1
	else
		::rowWin -= ::line - stbl
	endif
	::line := stbl
	::pos := leftbl
    endif
    ::cancelblock()
    ::updated := .t.
    if undo
	::refresh()
    endif
RETURN .t.
