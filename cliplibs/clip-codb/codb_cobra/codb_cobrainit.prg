************************************************************
function codb_CobraInit(iniFile,cUser,cPasswd,cHost,nPort)
	local ret:="",oIni, val,i,oQuery
	local values:= {;
		{"NPORT","TCP","port"},;
		{"CHOST","TCP","host"},;
		{"NIOTIMEOUT","TCP","io_timeout"},;
		{"NTIMEOUT","TCP","connect_timeout"};
		}
	if !empty(iniFile) .and. file(iniFile)
		oIni := iniFileNew(iniFile)
	endif
	if empty(oIni) .or. empty(iniFile)
		return [Ini-file not found:]+toString(iniFile)
	endif
	oIni:load()
	oServer:=cobraServiceNew(oIni)
	for i=1 to len(values)
		val := oIni:getValue(values[i][2],values[i][3])
		if !empty(val)
			oServer[values[i][1]] := val
		endif
	next
	if !empty(cHost)
		oServer:cHost := cHost
	endif
	if !empty(nPort)
		oServer:nPort := nPort
	endif
	oServer:cHost := alltrim(oServer:cHost)
	if ! oServer:connect()
		ret := oServer:error
		oServer:close()
		oServer := NIL
		return ret
	endif
	oServer:receiveLine()
	if "COBRA" $ upper(oServer:cLine)
	else
		oServer:close()
		oServer := NIL
		return [COBrA server NOT detected !]
	endif
	oQuery:=oServer:QueryNew()
	if !oServer:querySend(oQuery:id,"CLNT",oServer:info)
		ret := oServer:error
		oServer:queryDelete(oQuery:id)
		oServer:close()
		oServer := NIL
		return ret
	endif
	if !oServer:answerWait(oQuery:id)
		ret := oServer:error
		oServer:queryDelete(oQuery:id)
		oServer:close()
		oServer := NIL
		return ret
	endif
	if !empty(oQuery:answer:errno)
		ret := codb_cobraError(oQuery:answer)
		ret := oServer:error
		oServer:queryDelete(oQuery:id)
		oServer:close()
		oServer := NIL
		return ret
	endif
	oServer:info := oClone(oQuery:answer:return)
	if !empty(oQuery:answer:warning)
		oServer:warning += oQuery:answer:warning +","
	endif
	oServer:queryDelete(oQuery:id)
	if !empty(cUser)
		cUser := alltrim(cUser)
	endif
	if !empty(cPasswd)
		cPasswd := alltrim(cPasswd)
	endif
	if empty(cUser)
		return ""
	endif
	oQuery:=oServer:QueryNew()
	if !oServer:querySend(oQuery:id,"AUTH",cUser,cPasswd)
		ret := oServer:error
		oServer:queryDelete(oQuery:id)
		oServer:close()
		oServer := NIL
		return ret
	endif
	if !oServer:answerWait(oQuery:id)
		ret := oServer:error
		oServer:queryDelete(oQuery:id)
		oServer:close()
		oServer := NIL
		return ret
	endif
	if !oQuery:answer:return
		ret:=oQuery:answer:error
	endif
	if !empty(oQuery:answer:errno)
		ret := codb_cobraError(oQuery:answer)
	endif
	oServer:queryDelete(oQuery:id)
	if !empty(ret)
		oServer:close()
		oServer := NIL
	endif
return ret
