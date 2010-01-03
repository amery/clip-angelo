static function html_newLine(lBR)
        if lBR!=NIL .and. lBR
         	::addTag("BR")
        endif
	aadd(::tags,NIL)
return  len(::tags)
