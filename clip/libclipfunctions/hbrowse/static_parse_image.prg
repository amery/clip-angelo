static function parse_Image(tag)
	if "ALT"$tag:fields
		::str := "["+tag:fields:ALT+"]"
	else
		::str := "[IMG]"
	endif
	if "SRC"$tag:fields
	endif
return len(::str)
