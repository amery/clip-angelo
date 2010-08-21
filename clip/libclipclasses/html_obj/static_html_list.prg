static function html_List(lOn,ordering,other,compact,type)
	static lStack
        local n,m
	if lStack==NIL
		lStack:=stackNew()
        endif
	if lOn==NIL .or. lOn
        	if ordering!=NIL .and. ordering
        		m:=html_tagNew("OL",other)
                else
        		m:=html_tagNew("UL",other)
                endif
                if type!=NIL
                	m:addField("TYPE",type)
                endif
                if compact!=NIL .and. compact
                	m:addField("","COMPACT")
                endif
		n:=::addTag(m)
                lStack:push(n)
                ::newLine()
        else
        	n:=lStack:pop()
                if n!=NIL
                	n:=::closeTag(n)
                	::newLine()
                endif
        endif
return n
