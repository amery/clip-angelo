static function COBRA_queryASend(nQuery,cCommand,aParams)
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
	oQuery:args:=aclone(aParams)
	str+="QUERY "+alltrim(str(oQuery:id))+" "
	str+=var2Str(oQuery)
return	::sendLine(str)
