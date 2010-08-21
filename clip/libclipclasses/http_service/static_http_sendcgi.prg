static function HTTP_sendCgi(self,file,cmd,url,postData)
	local i,j,n,head:="",data,ret
	local out:=space(0), err:=space(0)
	local path:=""
	if !file(file)
		self:sendError(HTTP_ERR_FILENOTFOUND,file)
		return .f.
	endif

	i := atr("/",file)
	if i>0
		path := left(file,i)
	endif

	//aadd(self:sets,{"AUTH_TYPE",""})
	//aadd(self:sets,{"CONTENT_LENGTH",""})
	//aadd(self:sets,{"CONTENT_TYPE",""})
	//aadd(self:sets,{"PATH_INFO",""})
	//aadd(self:sets,{"PATH_TRANSLATED",""})
	//aadd(self:sets,{"REMOTE_IDENT",""})
	//aadd(self:sets,{"REMOTE_USER",""})

	cmd=strtran(cmd,"(","\(")
	cmd=strtran(cmd,")","\)")
	aadd(self:sets,{"GATEWAY_INTERFACE","CGI/1.1"})
	aadd(self:sets,{"QUERY_STRING",cmd})
	aadd(self:sets,{"SCRIPT_NAME","/cgi-bin"+url})
	aadd(self:sets,{"SCRIPT_FILENAME",file})

	if (ret:=syscmd(file+iif(fileIsScript(url)," "+cmd,""),@postData,@out,@err,self:sets,path)) != 0 .and. !empty(err)
		outlog("cgi-bin error:",file,cmd,err,ret)
		self:sendError(HTTP_ERR_INTERNALSERVERERROR,file+"?"+cmd+"<BR>"+err)
		return .f.
	endif
	self:sendAnswer("200 OK","",@out,ascDateTime(),,-1)
return .t.
