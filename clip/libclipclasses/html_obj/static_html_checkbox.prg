static function html_checkBox(name,value,checked,other)
	local ret,m
        m=html_tagNew("INPUT",other)
        m:addField("TYPE","CHECKBOX")
 	if name!=NIL
        	m:addField("NAME",name)
        endif
 	if value!=NIL
        	m:addField("VALUE",value)
        endif
 	if checked!=NIL .and. checked
        	m:addField("","CHECKED")
        endif
        ret:=::addTag(m)
        //::closeTag(ret)
        ::newLine()
return ret
