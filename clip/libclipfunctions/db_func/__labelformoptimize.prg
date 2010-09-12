/*
****************************************************************
procedure __LabelFormOptimize(cLBLName,lPrinter,cAltFile,lNoConsole,cFor,bFor,bWhile,;
	nNext,nRecord,lRest,lSample)

	local f,o,h

	if cfor == NIL .or. nNext != NIL
		__LabelForm(cLBLName,lPrinter,cAltFile,lNoConsole,bFor,bWhile,;
			nNext,nRecord,lRest,lSample)
	else
		if f := isfiltered()
			o := m6_FiltCopy(m6_GetAreaFilter())
			h := m6_NewFilter(cfor)
			m6_FiltJoin(m6_GetAreaFilter(),h,2)
		else
			m6_SetAreaFilter(m6_NewFilter(cfor))
		endif

		__LabelForm(cLBLName,lPrinter,cAltFile,lNoConsole,bFor,bWhile,;
			nNext,nRecord,lRest,lSample)

		dbclearfilter()
		if f
			m6_SetAreaFilter(o)
		endif
	endif
return
*/
