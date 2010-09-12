static function __SA_getLast()
	if len(::items)==0
		return NIL
	endif
	return atail(::items)[2]
return .t.
