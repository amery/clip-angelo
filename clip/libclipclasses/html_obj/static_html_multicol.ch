static function html_multiCol(text,cols,gutter,width)
	local ret,t
	t:=html_tagNew("MULTICOL")
        if cols!=NIL
        	t:addField("COLS",cols)
        endif
        if gutter!=NIL
        	t:addField("GUTTER",gutter)
        endif
        if width!=NIL
        	t:addField("WIDTH",width)
        endif
        ret:=::addTag(t)
        ::addData(text)
        ::closeTag(ret)
return ret
