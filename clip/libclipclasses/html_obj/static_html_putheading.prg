static function html_putHeading(text,Weight,align,lCenter)
	local ret,c,t
        lCenter:=iif(lCenter==NIL,.f.,lCenter)
        weight:=iif(weight==NIL,3,weight)
        if lCenter
        	c:=::addTag("CENTER")
        endif
        t:="H"+alltrim(str(weight),1,0)
        t:=html_tagNew(t)
        if align!=NIL
        	t:addField("ALIGN",align)
        endif
        ret:=::addTag(t)
        ::addData(text)
        ::closeTag(ret)
        if lCenter
        	::closeTag(c)
        endif
return ret
