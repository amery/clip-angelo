function m6_Search(cfor,bfor,var)
	local f,o,h

	if f := isfiltered()
		o := m6_FiltCopy(m6_GetAreaFilter())
		h := m6_NewFilter(cfor)
		m6_FiltJoin(m6_GetAreaFilter(),h,2)
	else
		m6_SetAreaFilter(m6_NewFilter(cfor))
	endif
	go top

	var := {}
	while !eof()
		aadd(var,recno())
		skip
	enddo

	dbclearfilter()
	if f
		m6_SetAreaFilter(o)
	endif
return NIL
