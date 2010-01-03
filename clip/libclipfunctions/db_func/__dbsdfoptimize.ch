function __dbSDFOptimize(lIO,file,rfields,cfor,bfor,uwhile,unext,rec,rest)
	local f,o,h,r

	if cfor == NIL .or. unext != NIL
		r := __dbSDF(lIO,file,rfields,bfor,uwhile,unext,rec,rest)
	else
		if f := isfiltered()
			o := m6_FiltCopy(m6_GetAreaFilter())
			h := m6_NewFilter(cfor)
			m6_FiltJoin(m6_GetAreaFilter(),h,2)
		else
			m6_SetAreaFilter(m6_NewFilter(cfor))
		endif

		r := __dbSDF(lIO,file,rfields,bfor,uwhile,unext,rec,rest)

		dbclearfilter()
		if f
			m6_SetAreaFilter(o)
		endif
	endif
return r
