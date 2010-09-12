static func __delAllColumns()
#ifdef DEBUG_CALL
	outlog(__FILE__,__LINE__,"delAllColumns")
#endif
	::colCount := 0
	asize(::__columns,0)
return .t.
