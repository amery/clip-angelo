static function me_runme()
local hKey, str, nchoice:=1, percent, mc, k

do while .t.
	hKey := inkey(0, INKEY_ALL)
	if hKey $ ::__macroKeys .and. ::__l_tobj
		mc := ::__macroKeys[hKey]
		if clip(mc:func, iif(mc:allWins, __SELF__, ::tobj[::curwin]))
			loop
		endif
	elseif hkey $ ::__mapKeys
		hkey := ::__mapKeys[hKey]
	endif

	if mLeftDown() .and. (mRow() == ::nBot .or. mRow() == ::nTop)
		hKey := ::CallMenu(@nChoice)

		if hKey == -1
			loop
		endif
	endif
	if mRightDown()
		hKey := ::CallPopupMenu()
	endif
	if !::__l_tobj .and. (hKey != HASH_CallMenu .and. ;
		hKey != HASH_OpenFile .and. ;
		hKey != HASH_CreateNewFile .and. ;
		hKey != HASH_ExitSave .and. ;
		hKey != HASH_ExitNoSave .and. ;
		hKey != HASH_HelpEdit)

		loop
	endif
	if ::__l_tobj
		if mLeftDown() .and. between(mRow(), ::nBot+1, ::nTop-1) .and. between(mCol(), ::nLeft+1, ::nRight-1)
			::tobj[::curwin]:mGoto(mRow(), mCol())
			loop
		endif
	endif
	/* run menu */
	if hKey == HASH_CallMenu
		hKey := ::CallMenu(@nChoice)

		if hKey == -1
			loop
		endif
	endif

	If hKey $ ::__macroCmd .and. !::__l_tobj
		loop
	elseif hKey $ ::__macroCmd .and. ::__l_tobj
		mc := ::__macroCmd[hKey]
		if clip(mc:func, iif(mc:allWins, __SELF__, ::tobj[::curwin]), @hKey)
			loop
		endif
	endif


	if (::single_mode .or. ::double_mode) .and. numAnd(kbdstat(), 0x3)>0 .and. ;
		!::tobjinfo[::curwin]:readOnly .and. ::tobj[::curwin]:draw(::single_mode, hKey)
		hKey := 0
		loop
	endif

	k := ::applyHash(hKey)
	if k == ME_EXIT
		exit
	//elseif k == ME_EXCEPTION  .and. ::tobj[::curwin]:handleKey(hKey)
		//loop
	elseif ((hKey>=32 .and. hKey<=256) .or. hKey==K_TAB) .and. !::tobjinfo[::curwin]:readOnly
	    if ( Set(_SET_INSERT) )
	       ::tobj[::curwin]:insert(chr(hKey))
	    else
	       ::tobj[::curwin]:overStrike(chr(hKey))
	    endif
	endif

	if ::nWins == 0
		::__l_tobj := .f.
	endif

	if ::curwin > 0 .and. ::__l_tobj
		dispbegin()
		percent := int(min(::tobj[::curwin]:line*100/::tobj[::curwin]:lines, 100))
		str:= replicate( translate_charset( __CHARSET__,host_charset(), substr(B_SINGLE, 6, 1)), ::tobj[::curwin]:__leninfo)
		@ -1, 1 say str color set("edit_colors_window")
		str:="<"+alltrim(str(::tobj[::curwin]:line))+"><"+alltrim(str(::tobj[::curwin]:pos))+"><"+alltrim(str(percent))+"%>"+iif(::tobj[::curwin]:updated, "*", "")
		@ -1, 1 say str color set("edit_colors_window")
		devpos(::tobj[::curwin]:nTop+::tobj[::curwin]:rowWin-1,::tobj[::curwin]:nLeft+::tobj[::curwin]:colWin-1)
		dispend()
	endif

enddo

return
