static function __run_codbselect(qArgs,oAnswer)
	local oDep,class_id,cExpr,index,numbers,tmp
		/* dep,class_name|class_id,cExpr,index,numbers*/
	if ! __check_args(qArgs,2,{"C","C","CU","CNU","NU"})
		oAnswer:errno := COBRA_ERR_BADARG
		return
	endif
	if !isFunction("codb_needDepository")
		oAnswer:errno := COBRA_ERR_CODB
		oAnswer:error := [CODB library not linked]
		return
	endif
	oDep := clip("codb_needDepository",qArgs[1])
	if empty(oDep)
		oAnswer:errno := COBRA_ERR_BADARG
		oAnswer:error := [Bad depository ID:]+qArgs[1]
		return
	endif
	if !empty(oDep:error)
		oAnswer:errno := COBRA_ERR_CODB
		oAnswer:error := oDep:error
		return
	endif
	class_id := clip("codb_metaIdByName",oDep,qArgs[2],"CLASS")
	if empty(class_id)
		oAnswer:errno := COBRA_ERR_BADARG
		oAnswer:error := [Bad class name or ID:]+qArgs[2]
		return
	endif
	if len(qArgs)>2
		cExpr := qArgs[3]
	endif
	if len(qArgs)>3
		index := qArgs[4]
	endif
	if len(qArgs)>4
		numbers := qArgs[5]
	endif
	tmp := oDep:select(class_Id,index,NIL,cExpr,numbers)
	if !empty(oDep:error)
		oAnswer:errno := COBRA_ERR_CODB
		oAnswer:error := oDep:error
	else
		oAnswer:return := tmp
	endif
return
