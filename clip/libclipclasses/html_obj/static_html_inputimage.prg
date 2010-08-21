static function html_inputImage(src,other,align,name)
	local ret,m
        m=html_tagNew("INPUT",other)
        m:addField("TYPE","IMAGE")
 	if src!=NIL
        	m:addField("SRC",src)
        endif
 	if align!=NIL
        	m:addField("ALIGN",align)
        endif
 	if name!=NIL
        	m:addField("NAME",name)
        endif
        ret:=::addTag(m)
        //::closeTag(ret)
        ::newLine()
return ret
