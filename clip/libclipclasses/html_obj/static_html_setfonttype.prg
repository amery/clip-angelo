static function html_setFontType(fType,lOn)
	static fStacks
        local n, hType, stack
        if fStacks==NIL
        	fStacks:=map()
        endif
	if ftype==NIL
        	return 0
        else
        	ftype=Upper(ftype)
                hType:=hashStr(ftype)
        	if ftype $ fStacks
                	stack:=fStacks[htype]
                else
			fStacks[hType]:=stackNew()
                	stack:=fStacks[htype]
                endif

        endif
	if lOn==NIL .or. lOn
		n:=::addTag(ftype)
                stack:push(n)
        else
        	n:=stack:pop()
                if n!=NIL
                	n:=::closeTag(n)
                endif
        endif
return n
