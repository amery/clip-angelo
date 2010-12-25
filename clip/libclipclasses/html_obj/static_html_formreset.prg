static function html_formReset(text,name,other)
	local ret,m
        m=html_tagNew("INPUT",other)
        m:addField("TYPE","RESET")
        if name!=NIL
        	m:addField("NAME",name)
        endif
        m:addField("VALUE",text)
        ret:=::addTag(m)
        //::closeTag(ret)
        ::newLine()
return ret
