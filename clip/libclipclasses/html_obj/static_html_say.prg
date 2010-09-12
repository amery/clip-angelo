static function html_say(str,font,size,type,color)
	local i,ss,ret
        ::__setFont(.t.,font,size,color)
        if type==NIL
        	type=""
        endif
        ss=html_tagSplit(type)
        for i=1 to len(ss)
                ::setFontType(ss[i],.t.)
        next

        ret:=::put(str)

        for i=len(ss) to 1 step -1
                ::setFontType(ss[i],.f.)
        next
        ::endFont()
return ret
