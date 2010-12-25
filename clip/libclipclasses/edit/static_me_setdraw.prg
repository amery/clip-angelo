************************************
static function me_setDraw(nWin, lMode)
* on/off single graphic mode
local obj := ::__checkWindow(@nWin)
	lMode := iif(lMode == NIL, .T., lMode)
	if lMode
		::double_mode := .f.
		::single_mode := !::single_mode
		if ::single_mode
			setcursor(2)
			::stline := [ESC - menu          <SINGLE GRAPHIC>]
		else
			setcursor(1)
			::stline := [ESC - menu ]
		endif
	else
		::double_mode := !::double_mode
		::single_mode := .f.
		if ::double_mode
			setcursor(2)
			::stline := [ESC - menu           <DOUBLE GRAPHIC>]
		else
			setcursor(1)
			::stline := [ESC - menu ]
		endif
	endif
	dispbegin()
	wselect(0)
	::oMenu:_status("CHECKED", HASH_SingleGraphic, ::single_mode)
	@ ::nBot, ::nLeft say padr(::stline, ::nRight-::nLeft+1) color "0/w"
	wselect(::tobjinfo[nWin]:curwin)
	::drawhead()
	devpos(obj:nTop+obj:rowWin-1,obj:nLeft+obj:colWin-1)
	dispend()
return .t.
