static function me_closeWindow(nWin)
local obj := ::__checkWindow(@nWin)
local i
	if obj:updated
		i := alert([File;]+::tobjinfo[nWin]:name+[;not saved. Save it?],{[Yes],[No],[Cancel]})
		if i==0 .or. i==3
			return .f.
		elseif i==1
			::saveFile()
		endif
	endif
	//::tobj[::curwin]:setFocus(.f.)
	obj:setFocus(.f.)
	wclose()

	::removeWindow(@nWin)
	do while ::nWins>0
		if ::tobj[nWin]!=NIL
			::__setFocus(nWin)
			wselect(::tobjinfo[::curwin]:curwin)
			::drawhead()
			//::tobj[::curwin]:refresh()
			exit
		endif
		nWin --
	enddo

	if len(::tobj) == 0
		::__l_tobj := .f.
		::curwin := 0
	endif
return .t.
