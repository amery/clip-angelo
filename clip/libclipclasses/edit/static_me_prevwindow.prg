static function me_prevWindow()

	do while .t.
		::__setFocus(::curwin - 1)
		if ::tobj[::curwin] != NIL
			exit
		endif
	enddo
	wselect(::tobjinfo[::curwin]:curwin)
	::drawhead()
	if ::tobjinfo[::curwin]:share
		::tobj[::curwin]:lines := len(::tobj[::curwin]:edbuffer)
		::tobj[::curwin]:refresh()
	endif
	devpos(::tobj[::curwin]:nTop+::tobj[::curwin]:rowWin-1, ::tobj[::curwin]:nLeft+::tobj[::curwin]:colWin-1)
return .t.
