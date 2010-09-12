static function db_home()
local homepos:=1
  if ::item_type == _GETB_
	::item:home()
	return
  elseif ::item_type == _GETTEXT_
	::item:edit:home()
	return
  endif
  ::killFocus()
  if len(::inbuffer[::line])>0
	homepos := ::inbuffer[::line][1]:nl
  endif
  if homepos == 1 .or. ::pos <= homepos
      ::pos := 1
      ::colWin := 1
  else
      ::colWin -= (::pos-homepos)
      ::pos := homepos
  endif
  ::refresh()
return
