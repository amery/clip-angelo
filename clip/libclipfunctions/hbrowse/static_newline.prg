static function newLine(step, real)
local col, ind
	//::doc:lines++
	step := iif(step==NIL, 1, step)
	real := iif(real==NIL, .f., real)
	if ::numTable<1 .or. real
		::doc:n_Line := ::doc:lines+step
		::doc:n_Pos := ::doc:marginLeft
	else
		::tblNewLine := .t.
	endif
return
