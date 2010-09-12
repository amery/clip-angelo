static function me_showFiles()
local obj, nWin
local k, r, s
	k := int(3*maxcol()/4)
	r := int(3*maxrow()/4)
	s := setcolor("15/7")//set("edit_colors_list"))
	//obj := ::tobj[::curwin]
	wopen(int((maxrow()-r)/2), int((maxcol()-k)/2), int((maxrow()-r)/2)+r, int((maxcol()-k)/2)+k)
	wbox()
	@ -1, (k-len([ Windows list ]))/2 say [ Windows list ]
	nWin := ::listfiles()
	wclose()
	::__setFocus(nWin)
	setcolor(s)
	if ::nWins == 0
		::__l_tobj := .f.
		::curwin := 0
	else
		obj := ::tobj[::curwin]
		wselect(::tobjinfo[::curwin]:curwin)
		::drawhead()
		if ::tobjinfo[::curwin]:share
			obj:lines := len(obj:edbuffer)
			obj:refresh()
		endif
		devpos(obj:nTop+obj:rowWin-1,obj:nLeft+obj:colWin-1)
	endif
return .t.
