static function me_findReplace(nWin)
local k, i, change
	::__checkWindow(@nWin)
	::__setFocus(nWin)
	k := ::tobj[::curwin]:curWord()
	::Find:fstring := iif(empty(k), ::Find:fhistory:first(), k)
	::Find:rstring := ::Find:rhistory:first()
	::Find:replace := .t.

	if !::Find:get()
		return .f.
	endif
	setcursor(1)
	if !empty(::Find:fstring)
		i := 0
		change := 0
		do while .t.
			k := 0
			do while k >= 0
				k := ::tobj[::curwin]:replace(::Find)
				if k == 1
					change++
				endif
			enddo

			if ::Find:allw
				::__setFocus(::curwin + 1)
				i++
				wselect(::tobjinfo[::curwin]:curwin)
				::drawhead()
				::tobj[::curwin]:gotoPos(1)
				::tobj[::curwin]:gotoLine(1)
				if i > len(::tobj)
					alert([String;]+::Find:fstring+[;not found!], "OK", set("edit_colors_menu"))
					exit
				endif
			else
				exit
			endif
		enddo
		alert([Made ]+alltrim(str(change))+ [ changes], "OK")
	endif
return .t.
