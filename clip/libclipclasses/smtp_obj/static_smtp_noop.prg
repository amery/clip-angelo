static function smtp_noop()
	local ret,buf := space(BUF_LEN)
	::error:=""
	if ::handle==NIL
		::error:=[Connect not activated]
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
