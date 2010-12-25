static function html_putImage(file,alt,border,height,name,target,other)
	local t:=html_tagNew("IMG",other)

        t:addField("SRC",file)
        if border!=NIL
        	t:addField("BORDER",border)
        endif
        if height!=NIL
        	t:addField("HEIGHT",height)
        endif
        if name!=NIL
        	t:addField("NAME",name)
        endif
        if alt!=NIL
        	t:addField("ALT",alt)
        endif
        if target!=NIL
        	t:addField("TARGET",target)
        endif
return ::addTag(t)
