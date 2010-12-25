function fx_scopeNext(oScope)
	if oScope == NIL
		oScope := atail(aScope)
	endif
	select (oScope:Select)
	if oScope:nCount != 0
		skip
	endif
	while !eof()
		oScope:nCount ++
		if oScope:bWhile != NIL .and. ! eval(oScope:bWhile)
			return .f.
		endif
		if oScope:nCount > oScope:nRecords
			return .f.
		endif
		if oScope:bFor == NIL .or. eval(oScope:bFor)
			return .t.
		endif
		skip
	enddo
return .f.
