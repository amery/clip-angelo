function __dbCopyOptimize(file,fields,cfor,bfor,uwhile,unext,rec,rest,rdd)
	local f,o,h,r

	if cfor == NIL .or. unext != NIL
		r := __dbCopy(file,fields,bfor,uwhile,unext,rec,rest,rdd)
	else
		if f := isfiltered()
			o := m6_FiltCopy(m6_GetAreaFilter())
			h := m6_NewFilter(cfor)
			m6_FiltJoin(m6_GetAreaFilter(),h,2)
		else
			m6_SetAreaFilter(m6_NewFilter(cfor))
		endif

		r := __dbCopy(file,fields,bfor,uwhile,unext,rec,rest,rdd)

		dbclearfilter()
		if f
			m6_SetAreaFilter(o)
		endif
	endif
return r
