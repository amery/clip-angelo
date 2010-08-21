static function smtp_reset()
	local ret,buf := space(BUF_LEN)

	::error:=""
	::fields := {}
	::attachments := {}
	if ::handle==NIL
		::error:=[Connect not activated]
		return .f.
	endif
	/* send RSET command to the server */
	tcpwrite(::handle,"RSET"+::EOL,,::timeout)
	if (ret:=tcpread(::handle,@buf,BUF_LEN,::timeout))<=0 .or. val(buf)>=500
		::error := ferrorstr()+" "+substr(buf,1,ret)
		tcpclose(::handle)
		return .f.
	endif
return .t.
