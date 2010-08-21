static function tcp_accept()
	local ret := -1
	ret := tcpAccept( ::listen, ::aTimeOut )
return ret
