static function html_frame(name,url,other,borderColor, frameBorder,;
			marginHeight,marginWidth,noResize,scrolling)
        local n,m
        m:=html_tagNew("FRAME",other)
        m:addField("NAME",name)
        m:addField("SRC",url)
        if borderColor!=NIL
        	m:addField("BORDERCOLOR",borderColor)
        endif
        if frameBorder!=NIL
        	m:addField("FRAMEBORDER",frameBorder)
        endif
        if marginHeight!=NIL
        	m:addField("MARGINHEIGHT",marginHeight)
        endif
        if marginWidth!=NIL
        	m:addField("MARGINWIDTH",marginWidth)
        endif
        if scrolling!=NIL
        	m:addField("SCROLLING",scrolling)
        endif
        if noResize!=NIL .and. noResize
        	m:addField("NORESIZE",noResize)
        endif
	n:=::addTag(m)
        ::newLine()
return n
