************************************************************
function codb_CobraQuery(cCmd)
	local i,oQuery,ret,aParams:={}
	for i = 1 to pcount()
		aadd(aParams,param(i))
	next
	oQuery:=oServer:QueryNew()
	if !oServer:queryASend(oQuery:id,"CODBUNIFY",aParams)
		ret := oServer:error
		oServer:queryDelete(oQuery:id)
		oServer:close()
		oServer := NIL
		return ret
	endif
	if !oServer:answerWait(oQuery:id)
		ret := oServer:error
		oServer:queryDelete(oQuery:id)
		oServer:close()
		oServer := NIL
		return ret
	endif
	ret := oQuery:answer
	oServer:queryDelete(oQuery:id)
return ret
