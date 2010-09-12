static function HTTP_runGet(self,cLine,postData)
	local i,j, url, _url, cmd:="", ver, file, flag,m
	local alias:="",path:=""
	outlog(__FILE__,__LINE__,self:command,cLine)
	i := at(" HTTP/",cLine)
	if i<=0
		self:sendError(HTTP_ERR_BADREQUEST,"HTTP/1.x not detected")
		return
	endif
	ver := val(substr(cLine,i+6))
	//outlog(__FILE__,__LINE__,ver)
	if ver < 1.0 .or. ver > 1.1
		self:sendError(HTTP_ERR_BADVERSION,ver)
		return
	endif
	url := alltrim(left(cLine,i))

	aadd(self:sets,{"REQUEST_METHOD",self:command})
	aadd(self:sets,{"REQUEST_URI",url})
	aadd(self:sets,{"UNIQUE_ID","CLIP"+ntoc(random(),32,10,"0")})
	if len(postData) > 0
		aadd(self:sets,{"CONTENT_LENGTH",alltrim(str(len(postData)))})
	endif

	//outlog(__FILE__,__LINE__,url)
	i:=at("?",url)
	if i > 0
		cmd := substr(url,i+1)
		url := substr(url,1,i-1)
	endif
	for i=1 to len(self:aliases)
		j := self:aliases[i][1]
		if left(url,len(j)) == j
			alias := j
			path  := self:aliases[i][2]
			exit
		endif
	next
	//outlog(__FILE__,__LINE__,alias)
	if ! empty(alias) //left(url,7) == "/icons/" .and. !empty(self:icons)
		set(_SET_ROOTPATH,path)
		url := substr(url,len(alias))
		file := makepath(url)
		if file(file)
			self:sendFile(url)
		else
			self:sendError(HTTP_ERR_FILENOTFOUND,url)
		endif
	elseif left(url,9) == "/cgi-bin/"
		url := substr(url,9)
		set(_SET_ROOTPATH,self:cgiBin)
		file := makepath(url)
		if file(file)
			self:sendCgi(file,cmd,url,@postData)
		else
			self:sendError(HTTP_ERR_FILENOTFOUND,url)
		endif
	elseif left(url,10) == "/mod-clip/"
		url := substr(url,10)
	//outlog(__FILE__,__LINE__,url,cmd,postdata)
		self:runModClip(url,cmd,@postData)
	else
		set(_SET_ROOTPATH,self:DocRoot)
		flag := .f.
		m := {"","/index.html","/index.htm","/index.shtml"}
		for i =1 to len(m)
			_url := url+m[i]
			file := makepath(_url)
	//outlog(__FILE__,__LINE__,_url,file,file(file))
			if !file(file)
				loop
			endif
	//outlog(__FILE__,__LINE__,"send",_url)
			self:sendFile(_url)
			flag := .t.
			exit
		next
		if !flag
			i := directory(url+"/*","D")
			if len(i)>2
				self:sendDir(url,cmd,i)
				flag := .t.
			endif
		endif
		if !flag
			self:sendError(HTTP_ERR_FILENOTFOUND,url)
		endif
	endif
return
