static function __SA_getData(pos)
	if !::__bound(pos)
		return NIL
	endif
	return ::items[pos][2]
return .t.
