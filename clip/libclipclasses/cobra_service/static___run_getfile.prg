static function __run_getfile(qArgs,oAnswer)
	local cFile
	if ! __check_args(qArgs,1,{"C"})
		oAnswer:errno := COBRA_ERR_BADARG
		return
	endif
	cFile := qArgs[1]
	if empty(cFile) .or. !file(makepath(cFile))
		oAnswer:errno := COBRA_ERR_BADFILENAME
		oAnswer:error := [File not found:]+makepath(cFile)
		return
	endif
	oAnswer:return := memoread(cFile)
	if ferror() !=0
		oAnswer:errno := ferror()
		oAnswer:error := ferrorstr()
	endif
return
