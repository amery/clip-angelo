function urlNew(prot,host,path,file,params,anchor,port,user,password)
	local obj:=map(),fill:=.f.

	if valType(prot)=="O"
		if prot:classname=="URL"
			obj:protocol	:= prot:protocol
			obj:host	:= prot:host
			obj:path	:= prot:path
			obj:file	:= prot:file
			obj:port	:= prot:port
			obj:user	:= prot:user
			obj:password	:= prot:password
			obj:params	:= prot:params
			obj:anchor	:= prot:anchor
		else
			obj:protocol	:= "http"
		endif
		fill:=.t.
	else
		obj:protocol	:= iif(prot==NIL, "http", prot )
	endif

	if !fill
		obj:host	:= iif(host==NIL, "",host )
		obj:path	:= iif(path==NIL, "", path )
		obj:file	:= iif(file==NIL, "index.html", file )
		obj:port	:= port
		obj:user	:= user
		obj:password	:= password
		obj:params	:= params
		obj:anchor	:= anchor
	endif

	obj:classname	:= "URL"
	obj:start	:= seconds()
	obj:stop	:= seconds()
	obj:pos		:= 0
	obj:status	:= ""
	obj:messages	:= ""
	obj:length	:= 0
	obj:ready	:= 0
	obj:__wget	:= NIL
	obj:__eof	:= .f.
	obj:__dEof	:= .f.
	obj:__fEof	:= .f.

	obj:parseUrl	:= @url_parseUrl()
	obj:makeUrl	:= @url_makeUrl()

	/* metods for read process from network */
	obj:open	:= @url_open()
	obj:close	:= @url_close()
	obj:eof		:= @url_eof()  // end of process reading file from network
	obj:kick	:= @url_kick()

	/* metods for read process from result file */
	obj:goTop	:= @url_goTop()
	obj:read	:= @url_read()
	obj:readStr	:= @url_readStr()
	obj:getStr	:= @url_getStr()
	obj:dataEof	:= @url_dataEof() // end of file with data
	obj:fileEof	:= @url_fileEof() // end of file with data in current
					//  state of process reading from network

	obj:destroy	:= @url_close()

return obj
