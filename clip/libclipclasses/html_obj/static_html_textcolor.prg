static function html_textColor(color)
	local ret
	if valtype(color)=="C" .and. !empty(color)
        	ret=::tags[::__body]:addField("TEXT",color)
                if ret!=0
                	return .t.
                endif
        endif
return .f.
