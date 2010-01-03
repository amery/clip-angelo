static function hp_end(s)
	if !empty(s)
		::buffer+=s
	endif
	::__parseBuf()
	if !empty(::buffer)
		aadd(::tags,::buffer)
		::buffer:=""
	endif
return .t.
