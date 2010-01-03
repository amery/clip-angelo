static func delColumn(pos)
	local oldcol:=::__columns[pos]
#ifdef DEBUG_CALL
	outlog(__FILE__,__LINE__,"delColumn")
#endif
	adel(::__columns,pos)
	::colCount--
	asize(::__columns,::colCount)
	if ::__firstStab
		::__remakeColumns()
	endif
return oldcol
