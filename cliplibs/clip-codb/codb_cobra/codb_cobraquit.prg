************************************************************
function codb_CobraQuit()
	if ! oServer:sendLine("QUIT")
		return oServer:error
	endif
	oServer:close()
	oServer := NIL
return ""
