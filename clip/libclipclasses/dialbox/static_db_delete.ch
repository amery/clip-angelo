static function db_delete()
	if ::item_type == _GETB_ .or. _GETTEXT_
		::item:delete()
	endif
return
