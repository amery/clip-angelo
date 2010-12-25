function dbStructInfo(sel,rec)
	local ret,i,oldsel:=select(),oldrec
	if sel!=NIL
		select(sel)
	endif
	oldrec:=recno()
	if rec!=NIL
		dbgoto(rec)
	endif
	ret:=dbstruct()
	for i=1 to len(ret)
		if len(ret[i]) == 4
			aadd(ret[i],.f.) // binary flag
			aadd(ret[i],.f.) // NULLable flag
		endif
		aadd(ret[i],fieldget(i))
	next
	dbgoto(oldrec)
	select(sel)
return ret
