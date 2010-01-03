static function COBRA_receive(nBytes)
	local nL,ret :=.f.,cBuf
	if ::nConnect < 0
		return .f.
	endif
	nBytes := iif( valtype(nBytes)=="N",nBytes,TCP_BUFLEN)
	cBuf := space(nBytes,.t.)
	nL := tcpRead( ::nConnect, @cbuf, nBytes, ::nIOtimeOut )
	if nL > 0
		ret := .t.
		::buffer += left(@cBuf,nL)
		::nReceived += nL
	else
		::errno := ferror()
		::error := fErrorStr()
	endif
return ret
