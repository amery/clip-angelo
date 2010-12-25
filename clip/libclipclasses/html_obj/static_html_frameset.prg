static function html_frameSet(lOn,other,cols,rows,border,borderColor,;
				frameBorder,frameSpacing)
	static fStack
        local n,m
	if fStack==NIL
		fStack:=stackNew()
        endif
	if lOn==NIL .or. lOn
        	m:=html_tagNew("FRAMESET",other)
                if cols!=NIL
                	m:addField("COLS",cols)
                endif
                if rows!=NIL
                	m:addField("ROWS",rows)
                endif
                if border!=NIL
                	m:addField("BORDER",border)
                endif
                if borderColor!=NIL
                	m:addField("BORDERCOLOR",borderColor)
                endif
                if frameBorder!=NIL
                	m:addField("FRAMEBORDER",frameBorder)
                endif
                if frameSpacing!=NIL
                	m:addField("FRAMESPACING",frameSpacing)
                endif
		n:=::addTag(m)
                fStack:push(n)
                ::newLine()
        else
        	n:=fStack:pop()
                if n!=NIL
                	n:=::closeTag(n)
                	::newLine()
                endif
        endif
return n
