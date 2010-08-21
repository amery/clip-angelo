static function db_end()
local len:=0

  if ::item_type == _GETB_
	::item:end()
	return
  elseif ::item_type == _GETTEXT_
	::item:edit:end()
	return
  endif
  ::killFocus()
  if ::line <= ::lines .and. len(::inbuffer[::line]) > 0
	len := ::inbuffer[::line][len(::inbuffer[::line])]:nr
  endif
  ::pos := len+1
  ::colWin := len+1
  ::refresh()
return
