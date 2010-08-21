static function html_THcell(text,other,align,backGround,bgColor,;
				borderColor,borderColorDark,borderColorLight,;
                                colspan,rowspan,nowrap,vAlign,width)

		local ret,m
        	m:=html_tagNew("TH",other)
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
                if nowrap!=NIL .and. nowrap
                	m:addField("","NOWRAP")
                endif
                if vAlign!=NIL
                	m:addField("VALIGN",valign)
                endif
                if width!=NIL
                	m:addField("WIDTH",width)
                endif
		ret:=::addTag(m)
                ::adddata(text)
                ::closeTag(ret)
                ::newLine()
return ret
