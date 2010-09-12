function recordInfo(sel,rec)
	local ret,k,x,i,oldsel:=select(),oldrec
		local __dbri_names:=__dbiInfo(2)
		if sel!=NIL .and. sel==0
			return map()
		endif
	if sel!=NIL
			select(sel)
		endif
		oldrec:=recno()
		if rec!=NIL
			dbgoto(rec)
		endif
		ret:=map()
		k:=mapKeys(__dbri_names)
		for i=1 to len(k)
			x:=k[i]
		ret[x]:=dbrecordInfo(x)
		next
		dbgoto(oldrec)
		select(sel)
return ret
