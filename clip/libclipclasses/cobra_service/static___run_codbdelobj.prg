static function __run_codbDelObj(qArgs,oAnswer)
	local tmp,oDep
	if ! __check_args(qArgs,1,{"C"})
		oAnswer:errno := COBRA_ERR_BADARG
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
	tmp := oDep:delete(qArgs[1])
	if !empty(oDep:error)
		oAnswer:errno := COBRA_ERR_CODB
		oAnswer:error := oDep:error
	else
		oAnswer:return := tmp
	endif
return
