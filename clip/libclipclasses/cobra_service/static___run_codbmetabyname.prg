static function __run_codbmetabyname(qArgs,oAnswer)
	local oDep
	if ! __check_args(qArgs,3,{"C","C","C"})
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
	oAnswer:return := oDep:metaBodyByName(qArgs[2],qArgs[3])
return
