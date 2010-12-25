static function html_Trow(lOn,other,align,border,backGround,bgColor,;
				borderColor,borderColorDark,borderColorLight,;
                                vAlign,width)
	static rStack
        local n,m
	if rStack==NIL
		rStack:=stackNew()
        endif
	if lOn==NIL .or. lOn
        	m:=html_tagNew("TR",other)
                if align!=NIL
                	m:addField("ALIGN",align)
                endif
                if border!=NIL
                	m:addField("BORDER",border)
                endif
                if backGround!=NIL
                	m:addField("BACKGROUND",backGround)
                endif
                if bgcolor!=NIL
                	m:addField("BGCOLOR",bgcolor)
                endif
                if borderColor!=NIL
                	m:addField("BORDERCOLOR",borderColor)
                endif
                if borderColorDark!=NIL
                	m:addField("BORDERCOLORDARK",borderColorDark)
                endif
                if borderColorLight!=NIL
                	m:addField("BORDERCOLORLIGHT",borderColorLight)
                endif
                if vAlign!=NIL
                	m:addField("VALIGN",vAlign)
                endif
                if width!=NIL
                	m:addField("WIDTH",width)
                endif
		n:=::addTag(m)
                rStack:push(n)
                ::newLine()
        else
        	n:=rStack:pop()
                if n!=NIL
                	n:=::closeTag(n)
                	::newLine()
                endif
        endif
return n
