static function db_killFocus()
local i
	if ::item_type == _GETB_
		::item:assign()
		::item:killFocus()
		::item:col := ::item_area[2]
		::item:row := ::item_area[1]
	elseif ::item_type == _CHECK_
		::item:varPut(::item:Control:buffer)
		::item:assign()
		::item:Control:killFocus()
		::item:Control:col := ::item_area[2]
		::item:Control:row := ::item_area[1]
	elseif ::item_type == _RADIO_
		::item:varPut(::item:Control:value)
		::item:assign()
		::item:Control:killFocus()
		for i=1 to ::item:Control:itemCount
			::item:Control:__items[i]:row := ::item_area[1][i]
			::item:Control:__items[i]:col := ::item_area[2][i]
		next
	elseif ::item_type == _GETTEXT_
		::item:assign()
		::item:killFocus()
		::item:edit:nTop := ::item_area[1]
		::item:edit:nLeft := ::item_area[2]
		::item:edit:nBottom := ::item_area[3]
		::item:edit:nRight := ::item_area[4]
	endif
	::item := map()
	::item:hasFocus := .f.
	::item:info := ""
	::item_type := -1
	asize(::item_area, 0)
	::item_num := 0
return
