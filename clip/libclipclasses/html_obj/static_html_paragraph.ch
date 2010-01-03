static function html_Paragraph(lOn,align,style)
	static pStack
        local n,p
	if pStack==NIL
		pStack:=stackNew()
        endif
	if lOn==NIL .or. lOn
        	if align!=NIL .or. style!=NIL
        		p:=html_tagNew("P")
                	if align!=NIL
                		p:addField("ALIGN",align)
                	endif
                	if style!=NIL
                		p:addField("STYLE",style)
                	endif
                        ::newLine()
			n:=::addTag(p)
                	pStack:push(n)
                endif
        else
        	n:=pStack:pop()
                if n!=NIL
                	::newLine()
                	n:=::closeTag(n)
                        ::newLine()
                endif
        endif
return n
