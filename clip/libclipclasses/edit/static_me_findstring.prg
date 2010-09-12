static function me_findString(nWin)
local k, r, curw
	::__checkWindow(@nWin)
	::__setFocus(nWin)
	k := ::tobj[::curwin]:curWord()
	::Find:fstring := iif (empty(k), ::Find:fhistory:first(), k)
	::Find:replace := .f.
	r := ::Find:get()
	if !r
		return .f.
	endif
	if ::Find:direct == 3
		::tobj[::curwin]:line := 1
		::tobj[::curwin]:pos := 1
	endif
	if ::Find:allw
		curw := ::curwin
		if !::tobj[::curwin]:find(::Find)
			do while .t.
				::__setFocus(::curwin + 1)
				::tobj[::curwin]:line := 1
				::tobj[::curwin]:pos := 0
				if iif(::Find:direct==2, ::tobj[::curwin]:findPrev(::Find), ::tobj[::curwin]:findNext(::Find))
					exit
				elseif ::curwin == curw
					alert([String;]+::Find:fstring+[;not found!], "OK", set("edit_colors_menu"))
					exit
				endif
			enddo
		endif
	   else
		if !::tobj[::curwin]:find(::Find)
			alert([String;]+::Find:fstring+[;not found!], "OK", set("edit_colors_menu"))
		endif
	endif
	setcursor(1)
return .t.
