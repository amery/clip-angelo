static function html_hLine(size,width)
	local ret,t
        t:=html_tagNew("HR")
        if size!=NIL
        	t:addField("SIZE",size)
        endif
        if width!=NIL
        	t:addField("WIDTH",width)
        endif
        ::newLine()
        ret:=::addTag(t)
        ::newLine()
return ret
