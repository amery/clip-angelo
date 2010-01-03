static function me_openFile()
local obj, nWin
local fd, sh, i, j, oldwin, shnum, afd, lCanReturnSelected := .t.

	if  ::__l_tobj .and. (::Opt:OOPENFILES == 1)
		i := atr('.', ::tobj[::curwin]:fileName)
		sh := iif(i > 0, "*"+substr(::tobj[::curwin]:fileName, i), "*")
		afd = fileDialog(::tobj[::curwin]:path, sh, ::__FileD, lCanReturnSelected)
	else
		afd=filedialog(__CurrentDir, "*", ::__FileD, lCanReturnSelected)
	endif

	if valtype(afd) != "A"
		fd := afd
		afd := {}
		aadd(afd, fd)
	endif
	for j=1 to len(afd)
		fd := afd[j]

		if !empty(fd)
			__CurrentDir := padr(fd, atr(PATH_DELIM, fd))
		endif
		sh := ::__check_share(fd, @shnum)
		if sh < 2
			return .f.
		endif
		if !empty(fd)
			setcolor(set("edit_colors_window"))
			oldwin := wselect()
			i := wopen(::ntop, ::nleft, ::nbot-1, ::nright)
			wbox()
			aadd(::tobjinfo, map())
			::nWins ++

			nWin := ::nWins
			::tobjinfo[nWin]:name := fd
			::tobjinfo[nWin]:readOnly := .f.
			::tobjinfo[nWin]:share := .f.
			::tobjinfo[nWin]:curwin := i //number Window

			aadd(::tobj, textEditNew(0, 0, maxrow(), maxcol(),set("edit_colors_edit")))
			obj := ::tobj[nWin]
			obj:modeFormat         := 2
			obj:line               := iif(::bline!=NIL, ::bline, obj:line)
			obj:pos                := iif(::bpos!=NIL,  ::bpos,  obj:pos)
			obj:rowWin             := iif(::wline!=NIL, ::wline, obj:rowWin)
			obj:colWin             := iif(::wpos!=NIL,  ::wpos,  obj:colWin)
			obj:marginRight        := ::Opt:OMARGINRIGHT
			obj:tabSize            := ::Opt:OTABSIZE
			obj:maxStrings         := ::Opt:OMAXSTRINGS
			obj:autoWrap           := ::Opt:OAUTOWRAP
			obj:lEofString         := .t.
			::__setFocus(nWin)
			if sh == 2
				::drawhead()
				if obj:loadFile(fd)
					::__l_tobj := .t.
				else
					alert([Can't open file;]+fd, "OK")
					asize(::tobj, ::nWins-1)
					asize(::tobjinfo, ::nWins-1)
					::nWins --
					wclose()
					::__setFocus(oldwin)
					wselect(::tobjinfo[::curwin]:curwin)
					::drawhead()
				endif
			else
				::tobjinfo[::curwin]:share := .t.
				::tobjinfo[shnum]:share := .t.
				::drawhead()
				::tobj[::curwin]:__nullInit()
				::tobj[::curwin]:edbuffer := ::tobj[shnum]:edbuffer
				::tobj[::curwin]:lines := len(::tobj[::curwin]:edbuffer)
				::tobj[::curwin]:refresh()
			endif
		endif
	next
return
