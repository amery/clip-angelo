static function tt_addCellToTable(tag)
local newCell, i, j, factCol

	::curCol ++
	 if ::curCol < len(::Titems[::curRow])
		for i=::curCol to len(::Titems[::curRow])
			if ::Titems[::curRow][i] == NIL
				exit
			endif
			::curCol ++
		next
	 endif

	newCell := HCellNew(tag, ::alignRow)
	factCol := ::curCol
	if len(::Tcolumns) < ::curCol
		aadd(::Tcolumns, 1)
	endif
	if len(::Titems[::curRow]) < ::curCol
		aadd(::Titems[::curRow], clone(newCell))
	else
		::Titems[::curRow][::curCol] := clone(newCell)
	endif

	for i=2 to newCell:colspan
		::curCol ++
		aadd(::Titems[::curRow], clone(newCell))
		if len(::Tcolumns) < ::curCol
			aadd(::Tcolumns, 1)
		endif
	next

	if newCell:colspan > 1
		::Titems[::curRow][::curCol]:colspan := 1
	endif

	**********

	for i=2 to newCell:rowspan
		if len(::Titems) < ::curRow+i-1
			::addRowToTable(,.f.)
		endif

		if len(::Titems[::curRow+i-1])<factCol
			asize(::Titems[::curRow+i-1], factCol+newCell:colspan-1)
		endif
		for j=1 to newCell:colspan
			::Titems[::curRow+i-1][factCol+j-1] := HCellNew()
			::Titems[::curRow+i-1][factCol+j-1]:rowspan := 0
			::Titems[::curRow+i-1][factCol+j-1]:colspan := 0
		next
		::Titems[::curRow+i-1][factCol]:colspan := newCell:colspan
		if newCell:colspan > 1
			::Titems[::curRow+i-1][newCell:colspan+factCol-1]:colspan := 1
		endif
	next
	if newCell:rowspan > 1
		for i=1 to newCell:colspan
			::Titems[newCell:rowspan+::curRow-1][factCol+i-1]:rowspan := 1
		next
	endif

return
