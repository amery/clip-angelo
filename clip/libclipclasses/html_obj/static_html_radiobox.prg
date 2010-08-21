static function html_radioBox(name,value,checked,other)
	local ret,m
        m=html_tagNew("INPUT",other)
        m:addField("TYPE","RADIO")
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
        ::newLine()
return ret
