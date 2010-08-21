static function db_enter(self, p1, p2, p3, p4, p5, p6, p7, p8, p9)
local oldcursor, oldcolor
	if ::item_type == _BUTTON_
		oldcursor:=setcursor()
		oldcolor := setcolor()
//		&(::item:userFunc)
		eval(::item:userFunc, 	self, p1, p2, p3, p4, p5, p6, p7, p8, p9)
		//restore screen from scr
		setcursor(oldcursor)
		setcolor(oldcolor)
		//::refresh()
	elseif ::item_type == _GETTEXT_
		::item:edit:insertLine()
	endif
return
