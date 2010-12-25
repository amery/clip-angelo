static function html_addData(str_data)
	if valtype(str_data)=="C"
		aadd(::tags,str_data)
		return  len(::tags)
        endif
return 0
