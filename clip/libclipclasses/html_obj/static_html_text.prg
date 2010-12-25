static function html_text(text,width,lWrap)
	local ret,t
	t:=html_tagNew("PRE")
        if width!=NIL
        	t:addField("WIDTH",width)
        endif
        if lWrap!=NIL
        	t:addField("",iif(lWrap,"WRAP","NOWRAP"))
        endif
        ret:=::addTag(t)
        ::newLine()
        ::addData(text)
        ::newLine()
        ::closeTag(ret)
        ::newLine()
return ret
