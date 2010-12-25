static func insColumn(pos,col)
#ifdef DEBUG_CALL
	outlog(__FILE__,__LINE__,"insColumn")
#endif
	aadd(::__columns,"")
	ains(::__columns,pos)
	::__columns[pos]:=col
	::colcount++
	if ::__firstStab
		::__remakeColumns()
	endif
return col
