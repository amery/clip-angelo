static function me_findPrev(nWin)
local curw
	::__checkWindow(@nWin)
	::__setFocus(nWin)
	if empty(::Find:fstring)
		alert([Nothing to find!!!], "OK")
	else
		if ::Find:allw
			curw := ::curwin
			if !::tobj[::curwin]:findPrev(::Find)
				do while .t.
					::__setFocus(::curwin + 1)
					::tobj[::curwin]:line := 1
					::tobj[::curwin]:pos := 0
					if ::tobj[::curwin]:findPrev(::Find)
						exit
					elseif ::curwin == curw
						alert([String;]+::Find:fstring+[;not found!], "OK", set("edit_colors_menu"))
						exit
					endif
				enddo
			endif
		else
			if !::tobj[::curwin]:findPrev(::Find)
				alert([String ]+::Find:fstring+[ not found!], "OK", set("edit_colors_menu"))
			endif
		endif
	endif
return .t.
