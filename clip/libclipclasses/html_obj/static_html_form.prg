static function html_Form(lOn,other,action,enctype,method,target)
	static fStack
        local n,m
	if fStack==NIL
		fStack:=stackNew()
        endif
	if lOn==NIL .or. lOn
        	m:=html_tagNew("FORM",other)
                if action!=NIL
                	m:addField("ACTION",action)
                endif
                if encType!=NIL
                	m:addField("ENCTYPE",enctype)
                endif
                if method!=NIL
                	m:addField("METHOD",method)
                else
                	m:addField("METHOD","POST")
                endif
                if target!=NIL
                	m:addField("TARGET",target)
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
