static function HTTP_waitRequest(self)
	local lExit := .f.,nl,cBuf,tm:=seconds()
	local crlf2 := chr(13)+chr(10)+chr(13)+chr(10)
	local hlen := 0, cLen := -1, len
	self:VMsetValues()
	self:cLine := ""
	while !lExit
		self:errno := 0
		cBuf := space(HTTP_NREAD,.t.)
		nL := tcpRead( self:nConnect, @cbuf, HTTP_NREAD, 1000 )
		if nL > 0
			self:nReceived += nL
			self:cLine+=left(cbuf,nl)
		endif
		if nl > 0 .and. hLen <= 0
			hLen := at(crlf2,self:cLine)
			if hLen > 0
				nl := at("Content-Length:",self:cLine)
				if nl > 0
					cLen := val(substr(self:cLine,nl+16,30))
				else
					cLen := 0
				endif
			endif
		endif
		if hLen > 0 .and. cLen == 0
			lExit := .t.
		endif
		if hLen > 0 .and. len(self:cline) >= hLen+3+cLen
			lExit := .t.
		endif
		if self:errno!=0
			lExit := .t.
		endif
		if seconds()-tm > self:nIoTimeOut/1000
			lExit := .t.
			self:errno := 11
		endif
	enddo
	if self:errno == 32
		self:error:=[Connection closed from client:]+ferrorStr()
		self:close()
		return .f.
	endif
	if self:errno == 11 /* request timeout */
		self:sendError(HTTP_ERR_REQUESTTIMEOUT)
		self:close()
		return .f.
	endif
return .t.
