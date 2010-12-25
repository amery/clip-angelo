static function me_initme()
local nfile, str, spl, i, fd, curwin, oldwin, home, percent, nWin
	::ntop := min(max(0,::ntop),maxrow())
	::nbot := min(max(0,::nbot),maxrow())
	::nleft := min(max(0,::nleft),maxcol())
	::nright := min(max(0,::nright),maxcol())

	::__mapKeys     := HK_get("edit")

	set("edit_margin_right","50") // ����� ������
	set("edit_page_len","20")     // ����� ��������
	set("edit_txtdir", curdir())   // ��� ��������� ���������
	set("edit_colors_edit",'11/1,0/2,0/3,0/7,1/3, 15/R') // ���� ��������������
	set("edit_colors_window",'14/1') // ���� ���� ��������������
	set("edit_colors_view",'0/7,7/1,0/3,,1/3')  // ���� ���������
	set("edit_colors_menu","0/7,0/2,r/7,g/rb,15/7,0/7") // ���� ����
	set("edit_colors_list","0/3,0/2,15/7,15/3,14/3,14/2,0/2")   // ���� list ��������
	//set("edit_colors_dialog","0/7,0/2,w+/b,g/rb,15/7,0/7")   // ���� ��������
	set("edit_colors_dialog","0/7,15/1,0/7,g/rb,15/3,0/7,0/7,15/7")   // ���� ��������
	set("edit_tabpack",'yes')   // ������� ������� � ���������
	set(_SET_SCOREBOARD,.t.)
	setcursor(1)
	//setcancel(.f.)
	::oMenu := ::menu()

	home := getenv("HOME")
	::__dirEdit := home+PATH_DELIM+".clip"
	::__fileOpt := home+PATH_DELIM+".clip"+PATH_DELIM+".edit"
	//::__fileStatus := "edit."+getenv("USER")
	::__fileStatus := home+PATH_DELIM+".clip"+PATH_DELIM+"we_status"+PATH_DELIM
	dirmake(::__fileStatus)
	::__fileStatus += strtran(currdrive()+"/"+curdir(), PATH_DELIM, ",")
	::__fileStatus += "."+getenv("USER")
	::__fileTempl := home+PATH_DELIM+".clip"+PATH_DELIM+".templ"
	::__fileMacro := home+PATH_DELIM+".clip"+PATH_DELIM+".macro"

	::set_options()

	::__FileD := historyObj(,,,,set("edit_colors_list"))
	::__FileD:setsize(::Opt:OHISTORY)
	::Find := findOptions(,,set("edit_colors_dialog"), set("edit_colors_list"), ;
		::Opt:FCASE, ::Opt:FWORDONLY, ::Opt:FREGEXP, ::Opt:FALLW, ::Opt:FDIRECT, ::Opt:FWHERE)
//      ::Find:sethistorysize(::Opt:OHISTORY)
	::__Hist := historyObj(,,,,set("edit_colors_list"))
	::__Hist:setsize(::Opt:OHISTORY)

	setcolor(set("edit_colors_window"))

	@ ::nTop, ::nLeft clear to ::nBot, ::nRight
	@ ::nBot, ::nLeft say padr(::stline, ::nRight-::nLeft+1) color "0/w"
	//::SB := scrollBar(::nTop, ::nBot, ::nRight, NIL)


	if !file(::__fileTempl)
		::__fileTempl := cliproot()+PATH_DELIM+"etc"+PATH_DELIM+".templ"
	endif
	if file(::__fileTempl)
		nfile := fopen(::__fileTempl)
		do while !fileeof(nfile)
			str := filegetstr(nfile, TE_MAX_LEN_STRING)
			spl := {}
			if search("[ &\t]+", str, spl)
				::templ[alltrim(upper(left(str, spl[1][1]-1)))] := substr(str, spl[1][2])
			endif
		enddo
		fclose(nfile)
	endif

	if !file(::__fileMacro)
		::__fileMacro := cliproot()+PATH_DELIM+"etc"+PATH_DELIM+".macro"
	endif
	if file(::__fileMacro)
		nfile := fopen(::__fileMacro)
		do while !fileeof(nfile)
			str := filegetstr(nfile, TE_MAX_LEN_STRING)
			spl := {}
			if search("[ &\t]+", str, spl)
				::tempmacro[alltrim(upper(substr(str, 1, spl[1][1]-1)))] := substr(str, spl[1][2])
			endif
		enddo
		fclose(nfile)
	endif

	__CurrentDir := currdrive()+PATH_DELIM+curdir()+PATH_DELIM

	dispbegin()
	if len(::param)>0
		::tobjinfo := {}
		for i:=1 to len(::param)
			fd := ::param[i]
			setcolor(set("edit_colors_window"))
			nWin := wopen(::ntop,::nleft,::nbot-1,::nright)
			wbox()
			aadd(::tobjinfo, map())
			::nWins ++
			::tobjinfo[nWin]:name := fd
			::tobjinfo[nWin]:readOnly := .f.
			::tobjinfo[nWin]:share := .f.
			::tobjinfo[nWin]:curwin := nWin

			aadd(::tobj, textEditNew(0, 0, maxrow(), maxcol(),set("edit_colors_edit")))
			::tobj[nWin]:modeFormat         := 2
			::tobj[nWin]:line               := iif(::bline!=NIL, ::bline, ::tobj[nWin]:line)
			::tobj[nWin]:pos                := iif(::bpos!=NIL,  ::bpos,  ::tobj[nWin]:pos)
			::tobj[nWin]:rowWin             := iif(::wline!=NIL, ::wline, ::tobj[nWin]:rowWin)
			::tobj[nWin]:colWin             := iif(::wpos!=NIL,  ::wpos,  ::tobj[nWin]:colWin)
			::tobj[nWin]:marginRight        := ::Opt:OMARGINRIGHT
			::tobj[nWin]:tabSize            := ::Opt:OTABSIZE
			::tobj[nWin]:maxStrings         := ::Opt:OMAXSTRINGS
			::tobj[nWin]:autoWrap           := ::Opt:OAUTOWRAP
			::tobj[nWin]:lEofString         := .t.
			if ::tobj[nWin]:loadFile(fd, .f.)
				::__l_tobj := .t.
				//::__setFocus(nWin)
				//::drawhead()
			else
				alert([Can't open file;]+fd, "OK")
				::nWins--
				asize(::tobj, ::nWins)
				asize(::tobjinfo, ::nWins)
				wclose()
			endif
		next
		if ::nWins >0
			oldwin := ::tobjinfo[1]:curwin
		else
			::__l_tobj := .f.
		endif
	else
		::getstatus()
		oldwin := ::curwin
		nWin := 0
		for i:=1 to ::nWins
			nWin ++
			::curwin := nWin
			fd := ::tobjinfo[i]:name
			setcolor(set("edit_colors_window"))
			::tobjinfo[nWin]:curwin := wopen(::ntop,::nleft,::nbot-1,::nright)
			wbox()
		     // ::drawhead()
			aadd(::tobj, textEditNew(0, 0, maxrow(), maxcol(),set("edit_colors_edit")))
			//::nWins ++
			::tobj[nWin]:modeFormat         := 2
			::tobj[nWin]:line               := iif(::bline!=NIL, ::bline, ::tobj[nWin]:line)
			::tobj[nWin]:pos                := iif(::bpos!=NIL,  ::bpos,  ::tobj[nWin]:pos)
			::tobj[nWin]:rowWin             := iif(::wline!=NIL, ::wline, ::tobj[nWin]:rowWin)
			::tobj[nWin]:colWin             := iif(::wpos!=NIL,  ::wpos,  ::tobj[nWin]:colWin)
			::tobj[nWin]:marginRight        := ::Opt:OMARGINRIGHT
			::tobj[nWin]:tabSize            := ::Opt:OTABSIZE
			::tobj[nWin]:maxStrings         := ::Opt:OMAXSTRINGS
			::tobj[nWin]:autoWrap           := ::Opt:OAUTOWRAP
			::tobj[nWin]:lEofString         := .t.
			if ::tobj[nWin]:loadFile(fd, .f.)
				::__l_tobj := .t.
				::tobj[nWin]:pos                := ::tobjinfo[i]:pos
				::tobj[nWin]:line               := ::tobjinfo[i]:line
				::tobj[nWin]:colWin             := ::tobjinfo[i]:colwin
				::tobj[nWin]:rowWin             := ::tobjinfo[i]:rowwin


				dispbegin()
				::tobj[nWin]:refresh()
				::drawhead()
				percent := int(::tobj[nWin]:line*100/::tobj[nWin]:lines)
				str:=padr("<"+alltrim(str(::tobj[nWin]:line))+"><"+alltrim(str(::tobj[nWin]:pos)) + ;
				"><"+alltrim(str(percent))+"%>", ::tobj[nWin]:__leninfo, translate_charset( __CHARSET__,host_charset(), + ;
				substr(B_SINGLE, 6, 1)))
				@ -1, 1 say str color set("edit_colors_window")
				devpos(::tobj[nWin]:nTop+::tobj[nWin]:rowWin-1,::tobj[nWin]:nLeft+::tobj[nWin]:colWin-1)
				setpos(::tobj[nWin]:rowwin-1, ::tobj[nWin]:colwin-1)
				dispend()
				//setpos(::tobj[nWin]:rowwin, ::tobj[nWin]:colwin)
			else
				alert([Can't open file;]+fd, "OK")
				adel(::tobj, nWin)
				adel(::tobjinfo, nWin)
				nWin --
				i --
				::nWins --
				asize(::tobj, len(::tobj)-1)
				asize(::tobjinfo, len(::tobjinfo)-1)
				wclose()
				if nWin > 0
					::curwin := nWin
					wselect(::tobjinfo[nWin]:curwin)
					::drawhead()
					devpos(::tobj[nWin]:nTop+::tobj[nWin]:rowWin-1,::tobj[nWin]:nLeft+::tobj[nWin]:colWin-1)
					setpos(::tobj[nWin]:rowwin-1, ::tobj[nWin]:colwin-1)
				endif
			endif
		next
	endif
	if valtype(::tobj) != "A"
		::tobj := {}
	endif
	if valtype(::tobjinfo) != "A"
		::tobjinfo := {}
	endif
	::nWins := len(::tobj)
	if ::nWins == 0
		::__l_tobj := .f.
		::curwin := 0
	else
		//::__setFocus(oldwin)
		::curwin := oldwin
	endif

	do while ::curwin > ::nWins
		::curwin --
	enddo
	if ::curwin > 0 .and. ::curwin<=::nWins
		dispbegin()
		wselect(::tobjinfo[::curwin]:curwin)
		::__setFocus(::curwin)
		//::drawhead()
		percent := int(::tobj[nWin]:line*100/::tobj[nWin]:lines)
		str:=padr("<"+alltrim(str(::tobj[nWin]:line))+"><"+alltrim(str(::tobj[nWin]:pos))+"><"+alltrim(str(percent))+"%>", + ;
		::tobj[nWin]:__leninfo, translate_charset( __CHARSET__,host_charset(),substr(B_SINGLE, 6, 1)))
		@ -1, 1 say str color set("edit_colors_window")
		devpos(::tobj[::curwin]:nTop+::tobj[::curwin]:rowWin-1,::tobj[::curwin]:nLeft+::tobj[::curwin]:colWin-1)
		setpos(::tobj[::curwin]:rowwin-1, ::tobj[::curwin]:colwin-1)
		::tobj[::curwin]:refresh()
		dispend()
	else
		::curwin := 1
	endif
	dispend()
	::Find:sethistorysize(::Opt:OHISTORY)
	::__setDefaultHash()    // set default command by hash
	::initUserMacro()
	setCursor(SC_INSERT)

return
