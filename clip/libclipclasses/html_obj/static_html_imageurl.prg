static function html_imageUrl(url,target,file,alt,border,height,name,other)
	local i,ret,t

	if url==NIL
        	return 0
        endif

        t:=html_tagNew("A")
        t:addField("HREF",url)
        if target!=NIL
        	t:addField("TARGET",target)
        endif
        ret:=::addTag(t)

	t:=html_tagNew("IMG",other)
        t:addField("SRC",file)
        if border!=NIL
        	t:addField("BORDER",border)
        endif
        if height!=NIL
        	t:addField("HEIGHT",height)
        endif
        if name!=NIL
        	t:addField("NAME",name)
        endif
        if alt!=NIL
        	t:addField("ALT",alt)
        endif
        i:=::addTag(t)
        ::closeTag(i)
        ::closeTag(ret)
return ret
