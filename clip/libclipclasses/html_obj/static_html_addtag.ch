static function html_addTag(tag_obj,p1,p2,p3,p4,p5,p6,p7,p8,p9,p10)
	if valtype(tag_obj)=="C"
        	tag_obj:=html_tagNew(tag_obj,p1,p2,p3,p4,p5,p6,p7,p8,p9,p10)
        endif
	if valtype(tag_obj)=="O"
		aadd(::tags,tag_obj)
		return  len(::tags)
        endif
return 0
