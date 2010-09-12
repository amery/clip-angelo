static function html_bgImage(image)
	local ret
	if valtype(image)=="C" .and. !empty(image)
        	ret=::tags[::__body]:addField("BACKGROUND",image)
                if ret!=0
                	return .t.
                endif
        endif
return .f.
