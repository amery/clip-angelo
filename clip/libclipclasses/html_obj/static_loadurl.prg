static function loadUrl(sUrl,timeout)
        local t,u:=urlNew(), pars:=htmlParserNew()
        timeout:=iif(valtype(timeout)!="N",60,timeout)
        u:parseUrl(sUrl)
        if !u:open()
        	::newLine()
        	::addData('Can~t open URL:"'+sUrl+'"')
        	::newLine()
        	::addData('Full URL/file is:"'+u:fileName+'"')
        	::newLine()
        endif
        do while !u:eof()
        	u:kick()
                if (seconds()-u:start) > timeout
                	exit
                endif
                if !u:eof()
                    sleep(1)
                endif
        enddo
        while !u:fileeof()
            t:=u:getstr()
            if len(t)==0
               exit
            endif
        enddo
        do while !u:fileeof()
        	pars:put( u:readstr(10000) )
        enddo
        pars:end(u:readstr(10000))
        u:close()
        ::tags:={}
        do while !pars:empty()
           t:=pars:get()
           if t==NIL
 		::newLine()
           else
           	if valtype(t)=="O"
                	::addTag(t)
                else
                	::addData(t)
                endif
           endif
        enddo
return .t.

