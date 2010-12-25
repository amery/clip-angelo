static function setColumns(col)
local Rcol:=::nRight, Lcol:=::nLeft
	if col == NIL .or. valtype(col) != "N" .or. col < 1
		return .f.
	endif
	::colWin := col
	while (Rcol-Lcol)%::colWin > 0
		Rcol --
	enddo
	asize(::lencol, ::colWin)
	afill(::lencol, int((Rcol-Lcol)/::colWin))
return .t.
