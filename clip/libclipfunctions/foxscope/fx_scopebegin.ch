function fx_scopeBegin(cfor,bfor,bwhile,nRecords,nRecord,lRest,lNoOptimize)
	local h,f,oScope
	if !used()
		return
	endif
	oScope	:= map()
	oScope:Select 	:= select()
	oScope:cFor 	:= cFor
	oScope:nRecords	:= iif(nRecords==NIL,lastrec(),nRecords)
	oScope:nCount	:= 0
	if valtype(nRecord)=="N"
		goto nRecord
		lRest := .t.
		oScope:nRecords := 1
	endif
	oScope:bWhile	:= bWhile
	oScope:bFor	:= bFor
	oScope:rm_filter:= NIL
	if lNoOptimize .or. nRecord != NIL
		oScope:cFor := NIL
	else
		if (f := isfiltered()) .and. !empty(cfor)
			oScope:rm_filter := m6_FiltCopy(m6_GetAreaFilter())
			h := m6_NewFilter(cfor)
			m6_FiltJoin(m6_GetAreaFilter(),h,2)
		elseif !empty(cfor)
			m6_SetAreaFilter(m6_NewFilter(cfor))
		endif
	endif

	if valtype(lRest)=="L" .and. lRest
	else
		goto top
	endif
	aadd(aScope,oScope)

return oScope
