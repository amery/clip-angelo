static function db_down()
	if ::item_type < _RADIO_
		::killFocus()
		::line++
		::rowWin++
		::refresh()
	elseif ::item_type == _RADIO_
		::item:Control:nextItem()
		::line++
		::rowWin++
	elseif ::item_type == _GETTEXT_
		::item:edit:down()
	endif
return
