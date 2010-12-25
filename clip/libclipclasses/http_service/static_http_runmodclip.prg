static function HTTP_runModClip(self,url,cmd,postData)
	local i,j,n,head:="",data,ret, file
	local cFile,cDate:=dt_create(),cData
	local out:=space(0), err:=space(0)
	local mod,oParams,cachable := .t., lastUpdated
	file := makepath(self:modclip+PATH_DELIM+url+".po")
	//outlog(__FILE__,__LINE__,file,file(file))
	if !file(file)
		self:sendError(HTTP_ERR_FILENOTFOUND,file)
		return .f.
	endif

	aadd(self:sets,{"GATEWAY_INTERFACE","MCI/1.0"})
	aadd(self:sets,{"QUERY_STRING",cmd})
	aadd(self:sets,{"SCRIPT_NAME","/mod-clip"+url})
	aadd(self:sets,{"SCRIPT_FILENAME",file})
	*****
	mod := loadBlock(file)
	//outlog(__FILE__,__LINE__,mod)
	if valtype(mod) != "B"
		self:sendError(HTTP_ERR_INTERNALSERVERERROR,file+" is not CLIP module.")
		return .f.
	endif
	if !empty(self:modCache)
		i := hashstr(file+cmd)
		i := ntoc(i,16,12,'0')
		cFile := makepath(self:modCache+PATH_DELIM+i)
		i := directory(cFile)
		if len(i)>0
			cDate := dt_create(i[1][3],i[1][4])
			cData := memoread(cFile)
		endif
	endif
	oParams := cgi_split(cmd,.t.)
	set(_SET_ROOTPATH,self:modclip)
	if !empty(self:modCache)
		cachable := eval(mod,"CACHE-ENABLED?",self,oParams,@postData)
		if valtype(cachable) == "L" .and. cachable
			lastUpdated := eval(mod,"LAST-UPDATED?",self,oParams,@postData)
		endif
	endif
	if dt_isdtdata(lastUpdated) .and. dt_isdtdata(cDate) .and. ;
	   dt_cmp(cDate,lastUpdated) > 0
		*** send old data from cache
		self:sendAnswer("200 OK","",cData,,,-1)
		return .t.
	endif

	ret := eval(mod,"RUN",self,oParams,@postData)
	if valtype(ret) == "L" .and. !ret
		self:sendError(HTTP_ERR_INTERNALSERVERERROR,file+"?"+cmd+"<BR>")
		return .f.
	endif
	cData := getmembuf()
	self:sendAnswer("200 OK","",@cData,,,-1)
	if !dt_isdtdata(lastUpdated)
		return .t.
	endif
	**** save data to cache file
	syscmd("cat > "+cFile,@cData,@out,@err)
return .t.
