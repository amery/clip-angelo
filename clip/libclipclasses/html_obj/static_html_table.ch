static function html_Table(lOn,other,align,border,backGround,bgColor,;
				borderColor,borderColorDark,borderColorLight,;
                                cellPadding,cellSpacing,frame,;
				hspace,rules,vSpace,width)
	static tStack
        local n,m
	if tStack==NIL
		tStack:=stackNew()
        endif
	if lOn==NIL .or. lOn
        	::newLine()
        	m:=html_tagNew("TABLE",other)
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
                if cellPadding!=NIL
                	m:addField("CELLPADDING",cellPadding)
                endif
                if cellSpacing!=NIL
                	m:addField("CELLSPACING",cellSpacing)
                endif
                if frame!=NIL
                	m:addField("FRAME",frame)
                endif
                if hspace!=NIL
                	m:addField("HSPACE",hspace)
                endif
                if rules!=NIL
                	m:addField("RULES",rules)
                endif
                if vspace!=NIL
                	m:addField("VSPACE",vspace)
                endif
                if width!=NIL
                	m:addField("WIDTH",width)
                endif
		n:=::addTag(m)
                tStack:push(n)
                ::newLine()
        else
        	n:=tStack:pop()
                if n!=NIL
                	n:=::closeTag(n)
                        ::newLine()
                endif
        endif
return n
