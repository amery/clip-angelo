function fx_eof(oScope)
	if oScope == NIL
	    oScope := atail(aScope)
	endif

	select (oScope:Select)
return eof()
