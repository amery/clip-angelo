function __DbListOptimize(off,fields,all,cfor,bfor,uwhile,unext,rec,rest,toPrint,toFile)
	local f,o,h,r

	if cfor == NIL .or. unext != NIL
		r := __DbList(off,fields,all,bfor,uwhile,unext,rec,rest,toPrint,toFile)
	else
		if f := isfiltered()
			o := m6_FiltCopy(m6_GetAreaFilter())
			h := m6_NewFilter(cfor)
			m6_FiltJoin(m6_GetAreaFilter(),h,2)
		else
			m6_SetAreaFilter(m6_NewFilter(cfor))
		endif

		r := __DbList(off,fields,all,bfor,uwhile,unext,rec,rest,toPrint,toFile)

		dbclearfilter()
		if f
			m6_SetAreaFilter(o)
		endif
	endif
return r
