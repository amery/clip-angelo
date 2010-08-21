static function db_bottom()
  if ::item_type == _GETTEXT_
	::item:edit:bottom()
	return
   endif
   ::killFocus()
   ::line:=::lines+1
   ::rowWin:=::nbottom-::ntop
   ::refresh()
return
