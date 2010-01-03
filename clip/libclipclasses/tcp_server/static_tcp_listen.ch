static function tcp_listen()
	::error := ""
	::errno := ""
	if valtype(::nPort) == "N" .and. ::nPort > 0
		::listen:= tcpListen( ::nPort, ::lTimeOut )
	endif
	if ( ::listen ) < 0
		::error:=[Error listen on port ]+toString(::nPort)+" "+ferrorStr()
		return .f.
	endif
return .t.
