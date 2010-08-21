function fx_scopeEnd(oScope)
	if oScope == NIL
		oScope := atail(aScope)
	endif
	select (oScope:Select)
	if !empty(oScope:cfor)
		dbclearfilter()
		if oScope:rm_filter != NIL
			m6_SetAreaFilter(oScope:rm_filter)
		endif
	endif
	asize(aScope, len(aScope)-1 )
return .t.
