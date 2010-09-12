static function smtp_connect()
	local ret,buf:=space(BUF_LEN)
	::handle := tcpconnect(::host,25,::timeout)
	if ::handle < 0 .or. ferror()!=0
		::error := ferrorstr()
		::handle := NIL
		return .f.
	endif
	/* read prompt from server */
	ret:=tcpread(::handle,@buf,BUF_LEN,::timeout)
	if ret<=0 .or. ferror()!=0
		tcpclose(::handle)
		return .f.
	endif
	/* check connect with server */
	tcpwrite(::handle,"NOOP"+::EOL,,::timeout)
	if (ret:=tcpread(::handle,@buf,BUF_LEN,::timeout))<=0 .or. val(buf)>=500
		::error := ferrorstr()+" "+substr(buf,1,ret)
		tcpclose(::handle)
		return .f.
	endif
return .t.
