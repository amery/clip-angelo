static function br_getUrl()
local buf, timeout:=60
	do while !::url:eof()
		::url:kick()
		if (seconds()-::url:start) > timeout
			exit
		endif
		if !::url:eof()
			sleep(1)
		endif
	enddo
	/*���������� ���������*/
	sleep(0.1)
	while  !::url:fileeof()
	    if upper(::url:protocol) == "FILE"
		exit
	    endif
	    buf:=::url:getstr()
		//alert(buf)
	    if len(buf)==0
	       exit
	    endif
	enddo

	sleep(0.1)
	do while !::url:fileEof()
		::url:kick()
		buf:=::url:readstr(10000)
		if empty(buf)
			sleep(0.1)
			loop
		endif
		::parser:put(buf)
		//outlog(buf)
	enddo
	::parser:end(::url:readStr(10000))
	::url:close()
return
