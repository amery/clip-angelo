static function html_url( text, url, target, other )
        local ret,t
        if empty(url)
        	return 0
        endif
        t:=html_tagNew("A",other)
        t:addField("HREF",url)
        if target!=NIL
        	t:addField("TARGET",target)
        endif
        ret:=::addTag(t)
        ::addData(text)
        ::closeTag(ret)
return ret
