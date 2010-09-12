static function __SA_getKey(pos)
	if !::__bound(pos)
		return NIL
	endif
	return ::items[pos][1]
return .t.
