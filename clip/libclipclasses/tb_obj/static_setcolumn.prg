static func setColumn(pos,col)
	local oldcol:=::__columns[pos]
#ifdef DEBUG_CALL
	outlog(__FILE__,__LINE__,"setColumn")
#endif
	::stable := .f.
	::__firstStab := .f.
	::__columns[pos]=col
	/*
	if ::__firstStab
	   ::__remakeColumns()
	endif
	*/
return oldcol
