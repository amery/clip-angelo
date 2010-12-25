static function html_marquee(lOn,other,align,behavior,bgcolor,direct,height,;
				hspace,loop,scrollAmount,scrollDelay,;
				vSpace,width)
	static mStack
        local n,m
	if mStack==NIL
		mStack:=stackNew()
        endif
	if lOn==NIL .or. lOn
        	::newLine()
        	m:=html_tagNew("MARQUEE",other)
                if align!=NIL
                	m:addField("ALIGN",align)
                endif
                if behavior!=NIL
                	m:addField("BEHAVIOR",behavior)
                endif
                if bgcolor!=NIL
                	m:addField("BGCOLOR",bgcolor)
                endif
                if direct!=NIL
                	m:addField("DIRECT",direct)
                endif
                if height!=NIL
                	m:addField("HEIGHT",height)
                endif
                if hspace!=NIL
                	m:addField("HSPACE",hspace)
                endif
                if loop!=NIL
                	m:addField("LOOP",loop)
                endif
                if scrollAmount!=NIL
                	m:addField("SCROLLAMOUNT",scrollAmount)
                endif
                if scrollDelay!=NIL
                	m:addField("SCROLLDELAY",scrollDelay)
                endif
                if vspace!=NIL
                	m:addField("VSPACE",vspace)
                endif
                if width!=NIL
                	m:addField("WIDTH",width)
                endif
		n:=::addTag(m)
                mStack:push(n)
                ::newLine()
        else
        	n:=mStack:pop()
                if n!=NIL
                	::newLine()
                	n:=::closeTag(n)
                        ::newLine()
                endif
        endif
return n
