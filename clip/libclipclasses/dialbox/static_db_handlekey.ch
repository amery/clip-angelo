static function db_handleKey(self, p1, p2, p3, p4, p5, p6, p7, p8, p9)
local hKey, wn, oldcursor, oldcolor, scr, oldcol,oldrow
local i, j, k, num, oInfo, nchoice:=1

	::drawBox()
	do while .t.
		hKey := inkey(0)
		if hkey $ ::mapKeys
			hkey := ::mapKeys[hKey]
		endif
		if hKey == HASH_CallMenu
			set( _SET_EVENTMASK, INKEY_ALL )
			i := setcursor(0)
			oldrow := row()
			oldcol := col()
			//oldwin := wi
			wopen(::nTop-1, ::nLeft, ::nBottom, ::nRight, .f.)
			oInfo := ::menu()
			hKey := MenuModal(oInfo,nchoice,maxrow(),0,maxcol(),"r/w")
			wclose()
			//wselect(::curwin)
			setpos(oldrow, oldcol)
			nchoice := oInfo:current
			setcursor(i)
			if hKey == -1
				loop
			endif
		endif
		do case
		case hKey == HASH_Exit
			::close()
			exit
		case hKey == K_UP
			::up()
		case hKey == K_DOWN
			::down()
		case hKey == K_LEFT
			::left()
		case hKey == K_RIGHT
			::right()
		case hKey == K_HOME
			::home()
		case hKey == K_END
			::end()
		case hKey == K_PGDN
			::pageDown()
		case hKey == K_PGUP
			::pageUp()
		case hKey == K_INS
			set(_SET_INSERT, !set(_SET_INSERT))
		case hKey == K_DEL
			::delete()
		case hKey == K_BS
			::backspace()
		case hKey == HASH_NextItem
			::select()
		case hKey == HASH_PrevItem
			::select(.f.)
		case hKey == K_ENTER
			::enter(self, p1, p2, p3, p4, p5, p6, p7, p8, p9)
		case hKey == K_F5
			num := ::line
			alert("Das ist Gluck into Screen!!!")
			readkeyb((::nBottom-::nTop)/2, ((::nRight-::nLeft)/2)-len([Line]), [Line], @num,,,"0/7,14/1", B_DOUBLE)
			setcursor(1)
			::goto(num)
			/*
		case hKey == HASH_Find
			num := 0
			alert("Das ist Gluck into Screen!!!")
			readkeyb((::nBottom-::nTop)/2, ((::nRight-::nLeft)/2)-len([Where are you going?]), [Where are you going?], @num,,,"0/7,14/1", B_DOUBLE)
			setcursor(1)
			if !::goItem(num)
				alert("Oblomaiz!!!")
			endif
			*/
		case hKey == HASH_Find
			k := ::curWord()
			::Find:fstring := iif (empty(k), ::Find:fhistory:first(), k)
			::Find:replace := .f.
			if ::Find:get()
				if !::findStr()
					alert([String;]+::Find:fstring+[;not found!], "OK", set("dbox_colors_menu"))
				endif
			endif
			setcursor(1)
		case hKey == HASH_ResumeFind
			::right()
			if !::findStr()
				alert([String;]+::Find:fstring+[;not found!], "OK", set("dbox_colors_menu"))
			endif
		case hKey == HASH_UnmarkBlock
			::cancelBlock()
		otherwise
		    ::insert(hKey)
		endcase
	enddo
return
