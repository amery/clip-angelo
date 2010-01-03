static function html_listItem(text,other,type,value)
	local ret,m
        m=html_tagNew("LI",other)
 	if type!=NIL
        	m:addField("TYPE",type)
        endif
 	if value!=NIL
        	m:addField("VALUE",value)
        endif
        ret:=::addTag(m)
        ::addData(text)
        ::closeTag(ret)
        ::newLine()
return ret
