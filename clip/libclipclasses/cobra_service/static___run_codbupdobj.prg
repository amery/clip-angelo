static function __run_codbUpdObj(qArgs,oAnswer)
	local tmp,cId
	if ! __check_args(qArgs,1,{"O","CU"})
		oAnswer:errno := COBRA_ERR_BADARG
		return
	endif
	if !isFunction("codb_getValue")
		oAnswer:errno := COBRA_ERR_CODB
		oAnswer:error := [CODB library not linked]
		return
	endif
	if len(qArgs)>1
		cId := qArgs[2]
	endif
	if valtype(cId) =="C"
		qArgs[1]:id := cId
	else
		if "ID" $ qArgs[1]
			cId := qArgs[1]:id
		else
			oAnswer:errno := COBRA_ERR_BADOBJID
			return
		endif
	endif
	tmp := clip("codb_update",qArgs[1])
	if tmp
		clip("codb_cacheDel",cId)
	endif
	oAnswer:return := tmp
return
