static function runing()
local hKey, wn, oldcursor, oldcolor, scr, oldrow, oldcol
local i, j, num, oInfo, nchoice:=1, old_translate_path, k

	old_translate_path := set(_SET_TRANSLATE_PATH, .f.)
	do while .t.
		hKey := inkey(0)
		if hkey $ ::mapKeys
			//if !::doc:item:hasFocus .or. ;
			//   (::doc:item:hasFocus .and. !search("[a-zA-Z/-]", chr(hKey)))
				hKey := ::mapKeys[hKey]
			//endif
		endif
		if hKey == HASH_CallMenu
			set( _SET_EVENTMASK, INKEY_ALL )
			i := setcursor(0)
			oldrow := row()
			oldcol := col()
			wopen(::nTop , ::nLeft, ::nBottom, ::nRight, .f.)
			oInfo := ::menu
			hKey := MenuModal(oInfo,nchoice,maxrow(),0,maxcol(),"r/w")
			wclose()
			setpos(oldrow, oldcol)
			nchoice := oInfo:current
			setcursor(i)
			if hKey == -1
				loop
			endif
		endif
		do case
		case hKey == HASH_Exit
		     //	::doc:close()
			exit
		case hKey == K_UP
			::doc:up()
		case hKey == K_DOWN
			::doc:down()
		case hKey == K_LEFT
			::doc:left()
		case hKey == K_RIGHT
			::doc:right()
		case hKey == K_HOME
			::doc:home()
		case hKey == K_END
			::doc:end()
		case hKey == K_PGDN
			::doc:pageDown()
		case hKey == K_PGUP
			::doc:pageUp()
		case hKey == K_INS
			set(_SET_INSERT, !set(_SET_INSERT))
		case hKey == K_DEL
			::doc:delete()
		case hKey == K_BS
			if !::doc:item:hasFocus
				::goBack()
			else
				::doc:backspace()
			endif
		case hKey == HASH_NextItem
			::doc:select()
		case hKey == HASH_PrevItem
			::doc:select(.f.)
		case hKey == K_ENTER
			::doc:enter(::doc, __self__)
		case hKey == K_F5
			num := ::doc:line
			alert("Das ist Gluck into Screen!!!")
			readkeyb((::nBottom-::nTop)/2, ((::nRight-::nLeft)/2)-len([Where are you going?]), [Where are you going?], @num,,,"0/7,14/1", B_DOUBLE)
			setcursor(1)
			::doc:goto(num)
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
			k := ::doc:curWord()
			::doc:Find:fstring := iif (empty(k), ::doc:Find:fhistory:first(), k)
			::doc:Find:replace := .f.
			if ::doc:Find:get()
				if !::doc:findStr()
					alert([String;]+::doc:Find:fstring+[;not found!], "OK", set("dbox_colors_menu"))
				endif
			endif
			setcursor(1)
		case hKey == HASH_ResumeFind
			::doc:right()
			if !::doc:findStr()
				alert([String;]+::doc:Find:fstring+[;not found!], "OK", set("dbox_colors_menu"))
			endif
		case hKey == HASH_UnmarkBlock
			::doc:cancelBlock()
		case hKey == HASH_NextUrl
			::goNext()
		case hKey == HASH_PrevUrl
			::goBack()
		case hKey == HASH_InfoUrl
			if !::doc:item:hasFocus
				alert("document reference:;"+::url:makeUrl(.f.), "OK", COLOR_DIALOG)
			elseif  ::doc:item_type == _BUTTON_
				alert("document reference:;"+::url:makeUrl(.f.)+";"+;
				      "current reference:;"+::doc:item:info, "OK", COLOR_DIALOG)
			else
				::doc:insert(chr(hKey))
			endif
		case hKey == HASH_SetCodePage
			::SetCodePage()
		otherwise
			::doc:insert(hKey)

		endcase
	enddo
	set(_SET_TRANSLATE_PATH, old_translate_path)

return
