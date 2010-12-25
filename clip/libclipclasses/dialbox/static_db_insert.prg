static function db_insert(hKey)
	if hKey == K_SPACE .and. ::item_type == _CHECK_
		::item:Control:select(!::item:Control:buffer)
	elseif ( ::item_type == _GETB_  .or. ::item_type == _GETTEXT_ ).and. hKey>=32 .and. hKey<=256
	       if set(_SET_INSERT)
			::item:Insert(chr(hKey))
	       else
			::item:overStrike(chr(hKey))
	       endif
	endif
return
