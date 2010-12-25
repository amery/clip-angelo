************************************

static function me_drawhead()
local nwin, l_nwin, ro, name, sh
	ro := ::tobjinfo[::curwin]:readOnly
	sh := ::tobjinfo[::curwin]:share
	name := ::tobjinfo[::curwin]:name
	nwin := iif(ro, [<R>], iif(sh, [<Link>], "")+"<"+iif(Set(_SET_INSERT), [I], [O])+">")+"["+alltrim(str(::tobjinfo[::curwin]:curwin))+"]"
	l_nwin := len(nwin)
	@ -1, maxcol()-l_nwin say  nwin color set("edit_colors_window")
	if !empty(name)
		dispbegin()
		name := right(name, maxcol()*3/4 - l_nwin)
		@ -1, maxcol()-maxcol()*3/4 say padc(name, maxcol()*3/4 - l_nwin, substr(B_SINGLE, 6, 1)) color set("edit_colors_window")
		dispend()
	endif
return .t.
