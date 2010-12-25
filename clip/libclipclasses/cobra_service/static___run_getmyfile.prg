static function __run_getmyfile(oConnect,qArgs,oAnswer)
	local ret := .t., oQuery := oConnect:queryNew()
	if ! __check_args(qArgs,1,{"C"})
		oAnswer:errno := COBRA_ERR_BADARG
		return
	endif
	if !oConnect:querySend(oQuery:id,"GETFILE",qArgs[1])
		ret := .f.
	endif
	if ret .and. !oConnect:answerWait(oQuery:id)//, len(oQuery:answer)
		ret := .f.
	endif
	if !ret
		oAnswer:errno := oConnect:errno
		oAnswer:error := oConnect:error
		oAnswer:return := .f.
		oConnect:queryDelete(oQuery:id)
		return
	endif
	if empty(oQuery:answer:errno)
		memowrit(qArgs[1],oQuery:answer:return)
		oAnswer:return := .t.
	else
		oAnswer:errno := oQuery:answer:errno
		oAnswer:error := oQuery:answer:error
		oAnswer:return := .f.
	endif
	oConnect:queryDelete(oQuery:id)
return
