static function html_formGet(type,name,value,size,maxlength,other)
	local ret,m
        m=html_tagNew("INPUT",other)
        m:addField("TYPE",type)
        m:addField("NAME",name)
        m:addField("VALUE",value)
        m:addField("SIZE",size)
        if maxlength!=NIL
        	m:addField("MAXLENGTH",maxLength)
        endif
        ret:=::addTag(m)
        //::closeTag(ret)
        ::newLine()
return ret
