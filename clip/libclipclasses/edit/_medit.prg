function _medit(par, ntop, nleft, nbot, nright, user_func,;
		  tab_size, bline, bpos, wline, wpos)
local obj:=map()
	obj:className   := "MEDIT"
	obj:tobj        := {}
	obj:tobjinfo    := {}
	obj:curwin      := 0
	obj:nWins       := 0
	obj:__l_tobj    := .f.
	obj:Clipboard   := {}
	//obj:SB                := 0
	obj:Find        := 0
	obj:__FileD     := 0
	obj:Opt         := 0
	obj:__Hist      := 0
	obj:templ       := map()
	obj:tempmacro   := map()
	obj:__dirEdit   := ""
	obj:__fileOpt   := ""
	obj:__fileStatus:= ""
	obj:__fileTempl := ""
	obj:__fileMacro := ""
	obj:userfunc    := user_func
	obj:ntop        := iif(ntop==NIL,0,ntop)
	obj:nbot        := iif(nbot==NIL,maxrow(),nbot)
	obj:nright      := iif(nright==NIL,maxcol(),nright)
	obj:nleft       := iif(nleft==NIL,0,nleft)
	obj:param       := iif(par==NIL .or. valtype(par)!="A", {}, par)
	obj:tab_size    := iif(tab_size==NIL, TE_TABSIZE, tab_size)
	obj:bline       := iif(bline==NIL,1,bline)
	obj:bpos        := iif(bpos==NIL,1,bpos)
	obj:wline       := iif(wline==NIL,1,wline)
	obj:wpos        := iif(wpos==NIL,1,wpos)
	obj:__mapKeys   := -1
	obj:single_mode := .f.
	obj:double_mode := .f.
	obj:stline      := [ESC -menu]
	obj:oMenu       := NIL
	obj:autoIndent  := .t.

	obj:__iniMacro  := NIL
	obj:__macroKeys := map()
	obj:__macroCmd  := map()
	obj:__hashKeys  := map()

	obj:__initMe    := @me_initme()
	obj:drawhead    := @me_drawhead()
	obj:getstatus   := @me_getstatus()
	obj:set_options := @me_set_options()
	obj:runme       := @me_runme()
	obj:saveFile    := @me_saveFile()
	obj:save_options:= @me_save_options()
	obj:writestatus := @me_writestatus()
	obj:listfiles   := @me_listfiles()
	obj:editopt     := @me_editopt()
	obj:goto        := @me_goto()
	obj:menu        := @me_menu_create()
	obj:enableMenu  := @me_enableMenu()
	obj:enableBlockMenu     := @me_enableBlockMenu()
	obj:__check_share       := @me___check_share()
	obj:helpEdit    := @me_helpEdit()
	obj:about       := @me_about()
	obj:setReadOnly := @me_setReadOnly()
	obj:getReadOnly := @me_getReadOnly()
	obj:chooseCodePage := @me_chooseCodePage()
	obj:changeCharset:= @me_changeCharset()
	obj:removeWindow := @me_removeWindow()
	obj:__setMacros := @me___setMacros()
	obj:__addMacroToMenu    := @me___addMacroToMenu()
	obj:initUserMacro:= @me__initUserMacro()

	obj:__setDefaultHash    := @me___setDefaultHash()
	obj:applyHash           := @me_applyHash()
	obj:setHash             := @me_setHash()
	obj:__checkWindow       := @me___checkWindow()
	obj:__setFocus          := @me___setFocus()

	obj:getFocus            := @me_getFocus()
	obj:saveWins            := @me_saveWins()
	obj:createNewWindow     := @me_createNewWindow()
	obj:openFile            := @me_openFile()
	obj:findString          := @me_findString()
	obj:findReplace         := @me_findReplace()
	obj:findPrev            := @me_findPrev()
	obj:findNext            := @me_findNext()
	obj:reloadFile          := @me_reloadFile()
	obj:prevWindow          := @me_prevWindow()
	obj:nextWindow          := @me_nextWindow()
	obj:setInsertMode       := @me_setInsertMode()
	obj:matchSymbol         := @me_matchSymbol()
	obj:matchStructure      := @me_matchStructure()
	obj:copyToClipboard     := @me_copyToClipboard()
	obj:saveBlock           := @me_saveBlock()
	obj:showFiles           := @me_showFiles()
	obj:closeWindow         := @me_closeWindow()
	obj:markBlock           := @me_markBlock()
	obj:cancelBlock         := @me_cancelBlock()
	obj:print               := @me_print()
	obj:undo                := @me_undo()
	obj:savePos             := @me_savePos()
	obj:callMenu            := @me_callMenu()
	obj:callPopupMenu       := @me_callPopupMenu()
	obj:newLine             := @me_NewLine()
	obj:delete              := @me_delete()
	obj:deleteEnd           := @me_deleteEnd()
	obj:backspace           := @me_backspace()
	obj:deleteHome          := @me_deleteHome()
	obj:deleteLine          := @me_deleteLine()
	obj:centerLine          := @me_centerLine()
	obj:copyBlock           := @me_copyBlock()
	obj:deleteBlock         := @me_deleteBlock()
	obj:pasteFromClipboard  := @me_pasteFromClipboard()
	obj:loadBlock           := @me_loadBlock()
	obj:insTemplate         := @me_insTemplate()
	obj:insMacro            := @me_insMacro()
	obj:format              := @me_format()
	obj:setDraw             := @me_setDraw()

	obj:__initMe()
	//obj:runme()
return obj
