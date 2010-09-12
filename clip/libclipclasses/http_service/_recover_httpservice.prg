function _recover_httpservice(obj)
	/* methods of class */
	obj:connect	:=@ HTTP_connect()
	obj:close	:=@ HTTP_close()
	obj:send	:=@ HTTP_send()
	obj:sendLine	:=@ HTTP_sendLine()
	obj:receive	:=@ HTTP_receive()
	obj:receiveLine	:=@ HTTP_receiveLine()
	obj:waitRequest	:=@ HTTP_waitRequest()
	obj:VMsetValues	:=@ HTTP_VMsetValues()
	obj:sendError	:=@ HTTP_sendError()
	obj:sendAnswer	:=@ HTTP_sendAnswer()
	obj:sendFile	:=@ HTTP_sendFile()
	obj:runSSI	:=@ HTTP_runSSI()
	obj:sendDir	:=@ HTTP_sendDir()
	obj:sendCgi	:=@ HTTP_sendCgi()
	obj:runModClip	:=@ HTTP_runModClip()
	obj:runCommand	:=@ HTTP_runCommand()
	obj:runGet	:=@ HTTP_runGet()
	obj:parseHeader :=@ HTTP_parseHeader()
	obj:checkVhost 	:=@ HTTP_checkVhost()

	obj:errorBlock	:=@ HTTP_errorBlock()

	obj:defSetting	:=@ HTTP_defSets()
	obj:fromIni	:=@ HTTP_fromIni()

	if valtype(obj:oIni) == "O"
		_recover_inifile(obj:oIni)
	endif
return obj
