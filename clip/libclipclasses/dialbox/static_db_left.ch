static function db_left()
	if ::item_type == _GETB_
		::item:left()
	elseif ::item_type == _GETTEXT_
		::item:edit:left()
	else
		if ::item_type == _CHECK_ .or. ::item_type == _RADIO_
			::killFocus()
		endif
		::pos--
		::colWin--
		::refresh()
	endif
return
