static function HTTP_connect()
	local ret:=.t.
	if ::nConnect > 0
		::close()
	endif
	::nConnect := tcpConnect( ::cHost, ::nPort, ::nTimeOut )
	if ::nConnect < 0
		::errno := ferror()
		::error := [TCP connection error:]+fErrorStr()
		ret := .f.
	endif
return ret
