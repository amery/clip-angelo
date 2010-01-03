static function html_addOption(value,text,other,selected)
	local ret,m
        m=html_tagNew("OPTION",other)
        m:addField("VALUE",value)
 	if selected!=NIL .and. selected
        	m:addField("","SELECTED")
        endif
        ret:=::addTag(m)
        ::addData(text)
        ::closeTag(ret)
        ::newLine()
return ret
