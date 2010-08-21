static function __run_codbUnify(qArgs,oAnswer)
	local i,cCmd, args:={}
	if valtype(qArgs) !="A" .or. len(qArgs)<1 .or. valtype(qArgs[1]) != "C"
		oAnswer:errno := COBRA_ERR_BADARG
		return
	endif
	cCmd := qArgs[1]
	for i=2 to len(qArgs)
		aadd(args,qArgs[i])
	next
	clip("codb_unify",cCmd,Args,oAnswer)
	if !empty(oAnswer:error) .and. empty(oAnswer:errno)
		oAnswer:errno := COBRA_ERR_CODB
	endif
return
