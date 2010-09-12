static function html_select(lOn,name,other,size,multiple)
	static sStack
        local n,m
	if sStack==NIL
		sStack:=stackNew()
        endif
	if lOn==NIL .or. lOn
        	m:=html_tagNew("SELECT",other)
                if name!=NIL
                	m:addField("NAME",name)
                endif
                if size!=NIL
                	m:addField("SIZE",size)
                endif
                if multiple!=NIL .and. multiple
                	m:addField("","MULTIPLE")
                endif
		n:=::addTag(m)
                sStack:push(n)
                ::newLine()
        else
        	n:=sStack:pop()
                if n!=NIL
                	n:=::closeTag(n)
                	::newLine()
                endif
        endif
return n
return n
