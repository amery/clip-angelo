static function smtp_hello(sData)
	local ret,buf := space(BUF_LEN)
	::error:=""
	if ::handle==NIL
		::error:=[Connect not activated]
		return .f.
	endif
	if !empty(sData)
		ret:=tcpwrite(::handle,"HELO "+sData+::EOL,,::timeout)
		if (ret:=tcpread(::handle,@buf,BUF_LEN,::timeout))<=0 .or. val(buf)>=500
			::error:=substr(buf,1,ret)
			return .f.
		endif
	endif
return .t.
