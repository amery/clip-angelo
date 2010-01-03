static function COBRA_querySend(nQuery,cCommand,arg1,arg2,arg3,arg4)
	local oQuery,i,ret := .t., str:=""
	if valtype(nQuery) != "N"
		ret := .f.
	endif
	if ret .and. ! (nQuery $ ::__queries)
		ret := .f.
	endif
	if ret
		oQuery := ::__queries[nQuery]
	endif
	if !ret .or. !__isQuery(oQuery)
		::errno := COBRA_ERR_BADQUERY
		::error := [Bad cobraQuery object]
		return .f.
	endif
	if !empty(cCommand)
		oQuery:command := cCommand
	endif
	for i=4 to pcount()
		aadd(oQuery:args,param(i))
	next
	str+="QUERY "+alltrim(str(oQuery:id))+" "
	str+=var2Str(oQuery)
return	::sendLine(str)
