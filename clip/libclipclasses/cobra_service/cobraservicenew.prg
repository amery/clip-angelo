function cobraServiceNew(oIni)
	local tmp,obj:=map()
	obj:classname	:= "COBRASERVICE"
	obj:cHost	:= COBRA_DEFHOST
	obj:nPort	:= COBRA_DEFPORT
	obj:nTimeOut	:= TCP_ACCEPT_TIMEOUT
	obj:nIOtimeOut	:= TCP_IO_TIMEOUT
	obj:nVersion	:= COBRA_VERSION
	obj:nConnect	:= -1
	obj:cLine	:= ""
	obj:buffer	:= ""
	obj:error	:= ""
	obj:warning	:= ""
	obj:errno	:= 0
	obj:nSended	:= 0
	obj:nReceived	:= 0
	obj:rootPath    := "./"

	obj:__queries	:= map()

	/* install values from ini-file */
	if valtype(oIni)=="O" .and. oIni:className == "INIFILE"

		tmp = oIni:getValue("COBRA","ROOTPATH")
		if !empty(tmp)
			tmp := strtran(tmp,"$CLIPROOT",cliproot())
		endif
		obj:rootPath := tmp
	endif

	obj:info	:= cobraInfoNew()
	obj:oIni	:= iniFileNew()
	obj:connect	:=@ COBRA_connect()
	obj:setRootPath	:=@ COBRA_setRootPath()
	obj:close	:=@ COBRA_close()
	obj:send	:=@ COBRA_send()
	obj:sendLine	:=@ COBRA_sendLine()
	obj:receive	:=@ COBRA_receive()
	obj:receiveLine	:=@ COBRA_receiveLine()
	obj:queryNew	:=@ COBRA_queryNew()
	obj:querySend	:=@ COBRA_querySend()
	obj:queryASend	:=@ COBRA_queryASend()
	obj:queryDelete	:=@ COBRA_queryDelete()
	obj:queryRun	:=@ COBRA_queryRun()
	obj:answerSend	:=@ COBRA_answerSend()
	obj:answerWait	:=@ COBRA_answerWait()

	obj:errorBlock	:=@ COBRA_errorBlock()
return obj
