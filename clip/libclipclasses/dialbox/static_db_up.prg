static function db_up()
	if ::item_type < _RADIO_
		::killFocus()
		::line--
		::rowWin--
		::refresh()
	elseif ::item_type == _RADIO_
		::item:Control:prevItem()
		::line--
		::rowWin--
	elseif ::item_type == _GETTEXT_
		::item:edit:up()
	endif
return
