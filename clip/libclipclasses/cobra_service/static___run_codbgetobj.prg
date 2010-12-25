static function __run_codbGetObj(qArgs,oAnswer)
	local tmp
	if ! __check_args(qArgs,1,{"C"})
		oAnswer:errno := COBRA_ERR_BADARG
		return
	endif
	if !isFunction("codb_getValue")
		oAnswer:errno := COBRA_ERR_CODB
		oAnswer:error := [CODB library not linked]
		return
	endif
	tmp := clip("codb_cacheGet",qArgs[1])
	if tmp == NIL
		tmp := clip("codb_getValue",qArgs[1])
	endif
	if !empty(tmp)
		clip("codb_cacheAdd",tmp:id,tmp)
	endif
	oAnswer:return := tmp
return
