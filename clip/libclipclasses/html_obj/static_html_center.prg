static function html_Center(lOn)
	static cStack
        local n
	if cStack==NIL
		cStack:=stackNew()
        endif
	if lOn==NIL .or. lOn
		n:=::addTag("CENTER")
                cStack:push(n)
        else
        	n:=cStack:pop()
                if n!=NIL
                	n:=::closeTag(n)
                endif
        endif
return n
