static function me___setDefaultHash()
local m
	::__hashKeys := map()
	m := ::__hashKeys


       m[HASH_ExitSave]                 := {|oMe, hKey| iif(oMe:saveWins() , (oMe:save_options(), iif( oMe:Opt:OSAVESTATUS,  oMe:writeStatus(), .f.), ME_EXIT), ME_CONTINUE) }
       m[HASH_ExitNoSave]               := {|oMe, hKey| iif(oMe:saveWins(), (oMe:save_options(), ME_EXIT), ME_CONTINUE)}
       /* create new window */
	m[HASH_CreateNewFile]           := {|oMe, hKey| oMe:__l_tobj := oMe:createNewWindow(), ME_CONTINUE }
       /* open window */
	m[HASH_OpenFile]                := {|oMe, hKey| oMe:openFile(), ME_CONTINUE }
	m[HASH_HelpEdit]                := {|oMe, hKey| oMe:helpEdit(), ME_CONTINUE }
	m[HASH_About]                   := {|oMe, hKey| oMe:about(), ME_CONTINUE }
       /* save file */
	m[HASH_SaveFile]                := {|oMe, hKey| oMe:saveFile(), ME_CONTINUE }
       /* save file as... */
	m[HASH_SaveAsFile]              := {|oMe, hKey| oMe:saveFile(.t.), ME_CONTINUE }
       /* find word */
	m[HASH_FindString]              := {|oMe, hKey| oMe:findString(), ME_CONTINUE }
       /* find word and replace */
	m[HASH_FindReplace]             := {|oMe, hKey| iif(oMe:tobjinfo[oMe:curwin]:readOnly, oMe:findString(), oMe:findReplace()), ME_CONTINUE }
       /* find next template to backward */
	m[HASH_FindPrev]                := {|oMe, hKey| oMe:findPrev(), ME_CONTINUE }
       /* find next template to forward */
	m[HASH_FindNext]                := {|oMe, hKey| oMe:findNext(), ME_CONTINUE }
       /* reload file */
	m[HASH_Reload]                  := {|oMe, hKey| oMe:reloadFile(), ME_CONTINUE }
       /* next window */
	m[HASH_NextWindow]              := {|oMe, hKey| oMe:nextWindow(), ME_CONTINUE }
       /* prev window */
	m[HASH_PrevWindow]              := {|oMe, hKey| oMe:prevWindow(), ME_CONTINUE }
       /* on/off insert mode */
	m[K_INS]                        := {|oMe, hKey| oMe:setInsertMode(), ME_CONTINUE }
	m[HASH_InsMode]                 := {|oMe, hKey| oMe:setInsertMode(), ME_CONTINUE }
       /* match forward symbol */
	m[HASH_MatchSymbolForward]      := {|oMe, hKey| oMe:matchSymbol(), ME_CONTINUE }
       /* match backward symbol */
	m[HASH_MatchSymbolBackward]     := {|oMe, hKey| oMe:matchSymbol(,.f.), ME_CONTINUE }
       /* match forward structure */
	m[HASH_MatchStructForward]      := {|oMe, hKey| oMe:matchStructure(), ME_CONTINUE }
       /* match backward structure */
	m[HASH_MatchStructBackward]     := {|oMe, hKey| oMe:matchStructure(, .f. ), ME_CONTINUE }
       /* copy block to Clipboard */
	m[HASH_CopyClipboard]           := {|oMe, hKey| oMe:copyToClipboard(oMe:curwin), ME_CONTINUE }
       /* save block to file */
	m[HASH_SaveBlock]               := {|oMe, hKey| oMe:saveBlock(), ME_CONTINUE }
       /* read only options on/off */
	m[HASH_ReadOnly]                := {|oMe, hKey| oMe:setReadOnly(oMe:curwin, !oMe:getReadOnly(oMe:curwin)), ME_CONTINUE }
       /* list opening files */
	m[HASH_ListFiles]               := {|oMe, hKey| oMe:showFiles(), ME_CONTINUE }
       /* close current window */
	m[HASH_CloseWindow]             := {|oMe, hKey| oMe:closeWindow(), ME_CONTINUE }
       /* edit options */
	m[HASH_Options]                 := {|oMe, hKey| oMe:editopt(), ME_CONTINUE }
       /* change code page */
	m[HASH_ChooseCodePage]          := {|oMe, hKey| oMe:chooseCodePage(), ME_CONTINUE }
       /* goto line/pos */
	m[HASH_GoTo]                    := {|oMe, hKey| oMe:goto(), ME_CONTINUE }
       /* save current line */
	m[HASH_SavePos]                 := {|oMe, hKey| oMe:savePos(), ME_CONTINUE }
       /* mark string block */
	m[HASH_MarkBlockString]         := {|oMe, hKey| oMe:markBlock(,.t.), ME_CONTINUE }
       /* mark rectangle block */
	m[HASH_MarkBlockRect]           := {|oMe, hKey| oMe:markBlock(,.f.), ME_CONTINUE }
       /* cancel block */
	m[HASH_CancelBlock]             := {|oMe, hKey| oMe:cancelBlock(), ME_CONTINUE }
       /* print text */
	m[HASH_Print]                   := {|oMe, hKey| oMe:print(,.f.), ME_CONTINUE }
       /* print block */
	m[HASH_PrintBlock]              := {|oMe, hKey| oMe:print(,.t.), ME_CONTINUE }
       /* run undo */
	m[HASH_Undo]                    := {|oMe, hKey| oMe:undo(), ME_CONTINUE }

	m[K_ENTER]                      := {|oMe, hKey| iif(!oMe:tobjinfo[oMe:curwin]:readOnly, oMe:newLine(), .f.), ME_CONTINUE }
       /* delete current symbol */
	m[K_DEL]                        := {|oMe, hKey| iif(!oMe:tobjinfo[oMe:curwin]:readOnly, oMe:delete(), .f.), ME_CONTINUE }
       /* delete string from current pos to end */
	m[HASH_DeleteEnd]               := {|oMe, hKey| iif(!oMe:tobjinfo[oMe:curwin]:readOnly, oMe:deleteEnd(), .f.), ME_CONTINUE }
       /* delete syblol brom left */
	m[K_BS]                         := {|oMe, hKey| iif(!oMe:tobjinfo[oMe:curwin]:readOnly, oMe:backSpace(), .f.), ME_CONTINUE }
       /* delete string from current pos to begin */
	m[HASH_DeleteBack]              := {|oMe, hKey| iif(!oMe:tobjinfo[oMe:curwin]:readOnly, oMe:deleteHome(), .f.), ME_CONTINUE }
       /* delete current line */
	m[HASH_DeleteLine]              := {|oMe, hKey| iif(!oMe:tobjinfo[oMe:curwin]:readOnly, oMe:deleteLine(), .f.), ME_CONTINUE }
       /* move string to the center */
	m[HASH_MoveCenter]              := {|oMe, hKey| iif(!oMe:tobjinfo[oMe:curwin]:readOnly, oMe:centerLine(), .f.), ME_CONTINUE }
       /* copy block */
	m[HASH_CopyBlock]               := {|oMe, hKey| iif(!oMe:tobjinfo[oMe:curwin]:readOnly, oMe:copyBlock(), .f.), ME_CONTINUE }
       /* move block */
	m[HASH_MoveBlock]               := {|oMe, hKey| iif(!oMe:tobjinfo[oMe:curwin]:readOnly, oMe:copyBlock(,.f.), .f.), ME_CONTINUE }
       /* remove block */
	m[HASH_DeleteBlock]             := {|oMe, hKey| iif(!oMe:tobjinfo[oMe:curwin]:readOnly, oMe:deleteBlock(), .f.), ME_CONTINUE }
       /* paste block from Clipboard */
	m[HASH_PasteClipboard]          := {|oMe, hKey| iif(!oMe:tobjinfo[oMe:curwin]:readOnly, oMe:pasteFromClipboard(), .f.), ME_CONTINUE }
       /* load block from file */
	m[HASH_LoadBlock]               := {|oMe, hKey| iif(!oMe:tobjinfo[oMe:curwin]:readOnly, oMe:loadBlock(), .f.), ME_CONTINUE }
       /* insert template */
	m[HASH_Template]                := {|oMe, hKey| iif(!oMe:tobjinfo[oMe:curwin]:readOnly, oMe:insTemplate(), .f.), ME_CONTINUE }
       /* insert macro */
	m[HASH_Macro]                   := {|oMe, hKey| iif(!oMe:tobjinfo[oMe:curwin]:readOnly, oMe:insMacro(), .f.), ME_CONTINUE }
       /* format line */
	m[HASH_FormatLine]              := {|oMe, hKey| iif(!oMe:tobjinfo[oMe:curwin]:readOnly, oMe:format(), .f.), ME_CONTINUE }
       /* format part */
	m[HASH_FormatPart]              := {|oMe, hKey| iif(!oMe:tobjinfo[oMe:curwin]:readOnly, oMe:format(,.f.), .f.), ME_CONTINUE }
       /* on/off single graphic mode */
	m[HASH_SingleGraphic]           := {|oMe, hKey| oMe:setDraw(,.t.), ME_CONTINUE }
       /* on/off double graphic mode */
	m[HASH_DoubleGraphic]           := {|oMe, hKey| oMe:setDraw(,.f.), ME_CONTINUE }

return .t.
