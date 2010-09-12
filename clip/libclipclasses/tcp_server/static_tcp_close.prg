static function tcp_close()
	if ::listen > 0
		tcpClose( ::listen )
		::listen := -1
		return .t.
	endif
return .f.
