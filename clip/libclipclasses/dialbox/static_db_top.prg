static function db_top()
  if ::item_type == _GETTEXT_
	::item:edit:top()
	return
  endif
  ::killFocus()
  ::line:=1
  ::rowWin:=1
  ::refresh()
return
