static function tt_addRowToTable(tag, incRow)
local iRow := map(), i
	incRow := iif(incRow==NIL, .t., incRow)
	if incRow
		::curRow ++
		::curCol := 0
	endif
	iRow:align  := ::alignRow
	iRow:row    := 1
	iRow:height := 1
	if tag != NIL
		if "ALIGN"$tag:fields
			::alignRow := hashstr(upper(tag:fields:ALIGN))
		endif
	endif
	if (len(::Titems) < ::curRow) .or. (!incRow .and. (len(::Titems) == ::curRow))
		aadd(::Titems, {})
		aadd(::Trows, clone(iRow))
	else
		if len(::Titems[::curRow]) > 0 .and. ::Titems[::curRow][1]!=NIL
			::curCol := len(::Titems[::curRow])
		endif

	endif
return
