static function db_setValue(numb, value)
local elem
	if ::goItem(numb, .f.)
		elem:=::inbuffer[::line][::item_num]
	else
		return .f.
	endif
	elem:objit:varPut(value)
	//elem:objit:assign()
	::refresh()
return
