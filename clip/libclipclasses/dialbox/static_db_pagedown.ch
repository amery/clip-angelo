static function db_pageDown()
	if ::item_type == _GETTEXT_
		::item:edit:pageDown()
		return
	endif
	::killFocus()
	::line := min(::line+::nbottom-::ntop, ::lines)
	::refresh()
return
