static function COBRA_close()
	if ::nConnect > 0
		tcpClose(::nConnect)
		::nConnect := -1
	endif
return .t.
