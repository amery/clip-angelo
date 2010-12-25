static function html_closeTag(tag_index)
	if valtype(tag_index)=="N"
        	if tag_index>0 .and. tag_index<=len(::tags)
			aadd(::tags,tag_index)
			return  len(::tags)
                endif
        endif
return 0
