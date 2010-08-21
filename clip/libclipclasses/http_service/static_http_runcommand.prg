static function HTTP_runCommand(self,file)
	local i,header,peerAddr,peerName,peerPort
	local data:=""
	local crlf := chr(13)+chr(10)

	tcpGetPeerName(self:nConnect, @peerAddr,@peerPort)
	peerName := GetHostByAddr(peerAddr)
	aadd(self:sets,{"REMOTE_HOST",peerName})
	aadd(self:sets,{"REMOTE_ADDR",peerAddr})
	aadd(self:sets,{"REMOTE_PORT",alltrim(str(peerPort))})

	/* data by POST command */
	i := at(crlf+crlf,self:cLine)
	if i!=0
		data:=substr(self:cLine,i+4)
		self:cLine :=left(self:cLine,i+3)
	endif

	i := at(" ",self:cLine)
	//outlog(__FILE__,__LINE__,self:cLine)
	if i<=0
		i := len(self:cLine)+1
	endif
	self:command := upper(left(self:cLine,i-1))
	self:cLine := alltrim(substr(self:cLine,i+1))
	*****
	i := at(chr(13)+chr(10),self:cLine)
	if i!=0
		header :=left(self:cLine,i-1)
		self:cLine:=substr(self:cLine,i+2)
	else
		header := self:cLine
	endif
	self:parseHeader()
	if self:command $ "GET POST"
		self:checkVhost()
	endif
	aadd(self:sets,{"SERVER_ADMIN",self:admin})
	aadd(self:sets,{"DOCUMENT_ROOT",self:DocRoot})
	*****
	if self:command $ "GET POST"
		self:runGet(Header,@data)
	else
		//self:send(space(400)+chr(13)+chr(10)+chr(13)+chr(10))
		self:sendError(HTTP_ERR_NOTIMPLEEMENTED,self:command)
	endif

	self:close()
return .t.
