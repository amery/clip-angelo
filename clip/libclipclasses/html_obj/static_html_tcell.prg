static function html_Tcell(lOn,other,align,backGround,bgColor,;
				borderColor,borderColorDark,borderColorLight,;
                                colSpan,rowSpan,noWrap,vAlign,width)
	static cStack
        local n,m
	if cStack==NIL
		cStack:=stackNew()
        endif
	if lOn==NIL .or. lOn
        	m:=html_tagNew("TD",other)
                if align!=NIL
                	m:addField("ALIGN",align)
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
                if colSpan!=NIL
                	m:addField("COLSPAN",colSpan)
                endif
                if rowSpan!=NIL
                	m:addField("ROWSPAN",rowSpan)
                endif
                if noWrap!=NIL .and. nowrap
                	m:addField("","NOWRAP")
                endif
                if vAlign!=NIL
                	m:addField("VALIGN",vAlign)
                endif
                if width!=NIL
                	m:addField("WIDTH",width)
                endif
		n:=::addTag(m)
                cStack:push(n)
                //::newLine()
        else
        	n:=cStack:pop()
                if n!=NIL
                	n:=::closeTag(n)
                	::newLine()
                endif
        endif
return n
