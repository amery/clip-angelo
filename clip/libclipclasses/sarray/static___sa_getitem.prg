static function __SA_getItem(pos)
	if !::__bound(pos)
		return NIL
	endif
	return ::items[pos]
return .t.
