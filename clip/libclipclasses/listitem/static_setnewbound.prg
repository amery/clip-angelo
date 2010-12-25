//static function setNewBound(top, left, bottom, right)
static function setNewBound(left, right)
	if valtype(left) != "N" .or. valtype(right) != "N" ;
		.or. (left < 0) .or. (right < 0)
		return .f.
	endif
	//::nTop := top
	::nLeft := left
	//::nBottom := bottom
	::nRight := right
	::SetColumns(::ColWin)
return .t.
