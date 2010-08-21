static function __run_auth(oInfo,qArgs,oAnswer,oIni)
	local cUser, cPasswd
	local cCmd,cStdOut:=space(0), cStdErr:=space(0)
	local s1,s2
	if ! __check_args(qArgs,2,{"C","C"})
		oAnswer:errno := COBRA_ERR_BADARG
		return
	endif
	cUser := qArgs[1]
	cPasswd := qArgs[2]
	cCmd := oInfo:auth
	if empty(cCmd)
		cCmd := COBRA_AUTH_SCRIPT
	endif
	if ( syscmd(cCmd,cUser+" "+cPasswd+"&\n",@cStdOut,@cStdErr) != 0 )
		oAnswer:errno := COBRA_ERR_SYSCMD
		oAnswer:error := cCmd+":"+cStdOut+" "+cStdErr
		oAnswer:return := .f.
	endif
	if empty(oAnswer:error)
		if left(cStdOut,2) == "OK"
			oAnswer:return := .t.
			oInfo:user   := cUser
			oInfo:passwd := cPasswd
		else
			oAnswer:errno := COBRA_ERR_AUTH
			oAnswer:return := .f.
			oAnswer:error := cCmd+":"+cStdOut+" "+cStdErr
		endif
	endif
	if !empty(oAnswer:error)
		s1 := oIni:getValue("TCP","AUTH-ERR-MSG")
		outlog(s1)
		if !empty(s1)
			s2 := memoread(s1)
		endif
		outlog(s2)
		if !empty(s2)
			oAnswer:error += "&\n&\n"+s2
		endif
	endif
return
