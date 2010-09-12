function tcpServerNew(cName,nPort,type,oIni)
	local tmp,obj := map()
	obj:className	:= "TCPSERVER"
	obj:error	:= ""
	obj:errno	:= 0
	obj:nPort	:= nPort
	obj:listen	:= -1

	if !empty(cName)
		obj:name	:= upper(cName)
	else
		obj:name	:= "TCPUNDEF"
	endif
	if !empty(type)
		obj:type	:= upper(type)
	else
		obj:type	:= "TCP"
	endif

	* timeOut for listen()
	obj:lTimeOut	:= TCP_LISTEN_TIMEOUT
	* timeOut for accept()
	obj:aTimeOut	:= TCP_ACCEPT_TIMEOUT

	if valtype(oIni)=="O" .and. oIni:className == "INIFILE"
		tmp := oIni:getValue(obj:type,"PORT")
		if valtype(tmp) == "N"
			obj:nPort := tmp
		endif
		tmp := oIni:getValue(obj:type,"LISTENTIMEOUT")
		if valtype(tmp) == "N"
			obj:lTimeOut := tmp
		endif
		tmp := oIni:getValue(obj:type,"ACCEPTTIMEOUT")
		if valtype(tmp) == "N"
			obj:aTimeOut := tmp
		endif
	endif
	obj:listen	:=@ tcp_listen()
	obj:close	:=@ tcp_close()
	obj:accept	:=@ tcp_accept()
return obj
