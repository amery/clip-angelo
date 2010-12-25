static function html_pageColor(color)
	local ret
	if valtype(color)=="C" .and. !empty(color)
        	ret=::tags[::__body]:addField("BGCOLOR",color)
                if ret!=0
                	return .t.
                endif
        endif
return .f.
