static function db_pageUp()
	if ::item_type == _GETTEXT_
		::item:edit:pageUp()
		return
	endif
	::killFocus()
	::line=::line-::nbottom-::ntop
	::refresh()
return
