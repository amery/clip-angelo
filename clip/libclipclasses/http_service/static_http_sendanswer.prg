static function http_sendAnswer(self,ret_code,type,data,modified,charset,nLen)
	local s:="" //,nLen
	local crlf:=chr(13)+chr(10)
	if valtype(ret_code) != "C"
		ret_code = "200 OK"
	endif
	/*
	if empty(type)
		type := "text/html"
	endif
	*/
	if !empty(type) .and. !empty(charset)
		type+="; charset="+charset
	endif
	s+="HTTP/1.1 "+ret_code+crlf
	s+="Date: "+ascDateTime()+crlf
	s+="Server: "+self:serverSoft+crlf
	if valtype(modified) == "C" .and. !empty(modified)
		s+="Last-Modified: "+modified+crlf
	endif
	if valtype(nLen) == "N" //.and. nLen >0
		if nLen == -1
			if left(data,2) == crlf
				nLen := len(data)-2
			elseif left(data,1) == chr(10)
				nLen := len(data)-1
			else
				nLen := at(chr(10)+chr(10),data)
				if nLen>0
					nLen := len(data)-nLen-1
				else
					nLen := len(data)-at(crlf+crlf,data)-3
				endif
			endif
		endif
		if nLen > 0
			s+="Accept-Ranges: bytes"+crlf
			s+="Content-Length: "+alltrim(str(nLen))+crlf
		endif
	endif
	s+="Connection: close"+crlf
	if !empty(type)
		s+="Content-Type: "+type+crlf
	endif
	if valtype(data) == "C"
		s+=data
	endif
	//outlog(__FILE__,__LINE__,"sended=",s)
	self:send(@s)
return .t.
