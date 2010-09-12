static function HTTP_sendFile(self,file)
	local i,ext,data,crlf:=chr(13)+chr(10)
	i := makePath(file)
	if file(i)
		data:=memoread(file)
	else
		self:sendError(HTTP_ERR_FILENOTFOUND,file)
		return .f.
	endif
	if empty(data) .and. !empty(ferror())
		data := crlf+"Can`t load data from "+file+", error:"+ferrorstr()
		self:sendAnswer("200 OK","text/plain",@data,"",,-1)
	else
		data := crlf+data
		i := atr(".",file)
		ext := substr(file,i+1)
		i := ascan(self:ssi,ext)
		if i>0
			data := self:runSSI(data)
		endif
		i := directory(file)
		self:sendAnswer("200 OK",mimeTypeGet(ext),@data,ascDateTime(i[1][3],i[1][4]),,len(data)-2)
	endif
return .t.
