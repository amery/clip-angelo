static function db_right()
	if ::item_type == _GETB_
		::item:right()
	elseif ::item_type == _GETTEXT_
		::item:edit:right()
	else
		if ::item_type == _CHECK_ .or. ::item_type == _RADIO_
			::killFocus()
		endif
		::pos++
		::colWin++
		::refresh()
	endif
return
