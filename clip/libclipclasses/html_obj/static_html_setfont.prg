static function html_setFont(lOn,face,size,color,lBold,lItalic,lUline)
	static fStack
        local n,font
	if fStack==NIL
		fStack:=stackNew()
        endif
	if lOn==NIL .or. lOn
        	if face!=NIL .or. size!=NIL .or. color!=NIL
        		font:=html_tagNew("FONT")
                	if face!=NIL
                		font:addField("FACE",face)
                	endif
                	if size!=NIL
                		font:addField("SIZE",size)
                	endif
                	if color!=NIL
                		font:addField("COLOR",color)
                	endif
			n:=::addTag(font)
                	fStack:push(n)
                endif
                if lBold != NIL
                	::setFontType("B",lBold)
                endif
                if lItalic != NIL
                	::setFontType("I",lItalic)
                endif
                if lUline != NIL
                	::setFontType("U",lUline)
                endif
        else
        	n:=fStack:pop()
                if n!=NIL
                	n:=::closeTag(n)
                endif
        endif
return n
