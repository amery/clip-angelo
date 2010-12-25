static function _m6_copyrec(fields,recnos)
	local rec := dbread(),i,a:={}

	if recnos==NIL .or. recnos
		aadd(a,recno())
	endif
	for i:=1 to len(fields)
		aadd(a,rec[fields[i][1]])
	next
return a
