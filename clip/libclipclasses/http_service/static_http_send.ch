static function HTTP_send(cData)
	local nL,ret := .f.
	if ::nConnect > 0 .and. valtype(cData)=="C"
		nL := tcpWrite( ::nConnect, @cData )
		if nl == len(cData)
			ret :=.t.
		else
			::errno := ferror()
			::error := fErrorStr()
		endif
		if nl>0
			::nSended += nL
		endif
	endif
return ret
