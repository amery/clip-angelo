function m6_dbeval(b,cfor,bfor)
	local h,o,f
	if !used()
		return
	endif
	if f := isfiltered()
		o := m6_FiltCopy(m6_GetAreaFilter())
		h := m6_NewFilter(cfor)
		m6_FiltJoin(m6_GetAreaFilter(),h,2)
	else
		m6_SetAreaFilter(m6_NewFilter(cfor))
	endif

	goto top

	while !eof()
		eval(b)
		skip
	enddo

	dbclearfilter()
	if f
		m6_SetAreaFilter(o)
	endif
return
