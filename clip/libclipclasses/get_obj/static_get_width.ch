static function get_width(new_width)
	local ret:=::__winlen
	if valtype(new_width)=="N" .and.new_width>0
	   ::__winlen := new_width
	endif
return  ret
