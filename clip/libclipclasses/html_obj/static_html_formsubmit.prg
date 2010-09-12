static function html_formSubmit(text,name,other)
	local ret,m
        m=html_tagNew("INPUT",other)
        m:addField("TYPE","SUBMIT")
        if name!=NIL
        	m:addField("NAME",name)
        endif
        m:addField("VALUE",text)
        ret:=::addTag(m)
        //::closeTag(ret)
        ::newLine()
return ret
