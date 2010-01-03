static function tb_hitTest(mrow,mcol)
	local i
	::mRowPos := ::rowPos
	::mColPos := ::colPos
	if mRow< ::__rect[1] .or. mRow > ::__rect[3]
		return HTNOWHERE
	endif
	if mCol < ::__rect[2] .or. mCol > ::__rect[4]
		return HTNOWHERE
	endif
	::mRowPos := mRow - ::__rect[1]+1
	for i = 1 to len(::__whereVisible)
		if ::__whereVisible[i] > mcol
			exit
		endif
	next
	::mColpos := ::__colVisible[i-1]
return HTCELL
