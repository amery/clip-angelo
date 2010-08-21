static function parse_Font(tag)
	if "COLOR" $ tag:fields
		::new_clr(::getHcolor(::clr, tag:fields:COLOR))
	endif
return
