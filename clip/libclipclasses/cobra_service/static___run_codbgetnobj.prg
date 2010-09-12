static function __run_codbGetNObj(qArgs,oAnswer)
	local i,tmp
	if ! __check_args(qArgs,1,{"A"})
		oAnswer:errno := COBRA_ERR_BADARG
		return
	endif
	if !isFunction("codb_getValue")
		oAnswer:errno := COBRA_ERR_CODB
		oAnswer:error := [CODB library not linked]
		return
	endif
	oAnswer:return := {}
	for i=1 to len(qArgs[1])
		tmp := clip("codb_cacheGet",qArgs[1][i])
		if tmp == NIL
			tmp := clip("codb_getValue",qArgs[1][i])
		endif
		if tmp != NIL
			clip("codb_cacheAdd",tmp:id,tmp)
		endif
		aadd(oAnswer:return,tmp)
	next
return
