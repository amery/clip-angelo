************************************
static function me_callMenu(nChoice)
local i, oldrow, oldcol, hKey

	set( _SET_EVENTMASK, INKEY_ALL )
	i := setcursor(0)
	oldrow := row()
	oldcol := col()
	wopen(::nTop, ::nLeft, ::nbot, ::nright, .f.)
	__keyboard(13)
	::enableMenu()
	::enableBlockMenu()
	hKey := MenuModal(::oMenu,nChoice,maxrow(),0,maxcol(),"r/w")
	wclose()
	if ::nWins > 0
		wselect(::tobjinfo[::curwin]:curwin)
	endif
	setpos(oldrow, oldcol)
	setcursor(i)
	nChoice := ::oMenu:current

return hKey
