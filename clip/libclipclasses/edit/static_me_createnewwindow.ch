static function me_createNewWindow()
local i, nWin

	setcolor(set("edit_colors_window"))
	i=wopen(::ntop, ::nleft, ::nbot-1, ::nright)
	wbox()

	aadd(::tobjinfo, map())
	::nWins++
	nWin := ::nWins
	::tobjinfo[nWin]:name := ""
	::tobjinfo[nWin]:readOnly := .f.
	::tobjinfo[nWin]:share := .f.
	::tobjinfo[nWin]:curwin := i //number Window
	::tobjinfo[nWin]:updated := .f.

	aadd(::tobj, textEditNew(0, 0, maxrow(), maxcol(),set("edit_colors_edit")))
	::tobj[nWin]:modeFormat             := 2
	::tobj[nWin]:line                   := iif(::bline!=NIL, ::bline, ::tobj[nWin]:line)
	::tobj[nWin]:pos                    := iif(::bpos!=NIL,  ::bpos,  ::tobj[nWin]:pos)
	::tobj[nWin]:rowWin                 := iif(::wline!=NIL, ::wline, ::tobj[nWin]:rowWin)
	::tobj[nWin]:colWin                 := iif(::wpos!=NIL,  ::wpos,  ::tobj[nWin]:colWin)
	::tobj[nWin]:marginRight            := ::Opt:OMARGINRIGHT
	::tobj[nWin]:tabSize                := ::Opt:OTABSIZE
	::tobj[nWin]:maxStrings             := ::Opt:OMAXSTRINGS
	::tobj[nWin]:autoWrap               := ::Opt:OAUTOWRAP
	::tobj[nWin]:lEofString             := .t.
	::tobj[nWin]:loadString("")
	::__setFocus(nWin)
return .t.
