static function db_backspace()
	if ::item_type == _GETB_ .or. ::item_type == _GETTEXT_
		::item:backspace()
		return .t.
	endif
return .f.
