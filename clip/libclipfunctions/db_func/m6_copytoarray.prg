function m6_CopyToArray(var,fields,cfor,bfor,bwhile,next,recno,rest,recnos)
	local f,o,h,dstru:=array(0),sstru := dbstruct(),i,j,rec,line,x

	if len(fields) == 0
		dstru := sstru
	else
		for j:=1 to len(fields)
			for i:=1 to len(sstru)
				if upper(sstru[i][1]) == alltrim(upper(fields[j]))
					aadd(dstru,sstru[i])
					exit
				endif
			next
		next
	endif
	if f := isfiltered()
		o := m6_FiltCopy(m6_GetAreaFilter())
		h := m6_NewFilter(cfor)
		m6_FiltJoin(m6_GetAreaFilter(),h,2)
	else
		m6_SetAreaFilter(m6_NewFilter(cfor))
	endif

	var := {}
	if recno!=NIL .and. recno > 0
		goto recno
		aadd(var,_m6_copyrec(dstru,recnos))
	else
		if bwhile != NIL
			rest := .t.
		endif
		if empty(rest) .and. next==NIL //rest==NIL .or. !rest
			go top
		endif
		while !eof()
			aadd(var,_m6_copyrec(dstru,recnos))
			if next!=NIL .and. len(var)==next .or.;
				bwhile!=NIL .and. !eval(bwhile)
				exit
			endif
			skip
		enddo
	endif
	dbclearfilter()
	if f
		m6_SetAreaFilter(o)
	endif
return NIL
