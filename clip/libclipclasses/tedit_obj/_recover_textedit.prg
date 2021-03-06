function _recover_textedit(obj)
       obj:__nullInit   := @__te_nullInit()

       obj:down         := @te_down()
       obj:up           := @te_up()
       obj:left         := @te_cleft()
       obj:right        := @te_cright()
       obj:wordLeft     := @te_wordLeft()
       obj:wordRight    := @te_wordRight()
       obj:Bottom       := @te_goBottom()
       obj:Top          := @te_goTop()
       obj:home         := @te_home()
       obj:end          := @te_end()
       obj:pageDown     := @te_pageDown()
       obj:pageUp       := @te_pageUp()
       obj:prevPage     := @te_prevPage()
       obj:nextPage     := @te_nextPage()

       //obj:panHome      := @te_panHome()
       //obj:panEnd       := @te_panEnd()
       //obj:panLeft      := @te_panLeft()
       //obj:panRight     := @te_panRight()
       //obj:panUp        := @te_panUp()
       //obj:panDown      := @te_panDown()

       obj:handleKey    := @te_handleKey()

       obj:gotoLine     := @te_gotoLine()
       obj:gotoPos      := @te_gotoPos()
       obj:mgoto        := @te_mgoto()
       obj:find         := @te_find()
       obj:findNext     := @te_findNext()
       obj:findPrev     := @te_findPrev()
       obj:replace      := @te_replace()
       obj:curWord      := @te_curWord()
       obj:prevWord     := @te_prevWord()
       obj:nextWord     := @te_nextWord()
       obj:Identity     := @te_Identity()
       obj:matchStruct  := @te_matchStruct()

       obj:clear        := @te_clear()
       obj:loadString   := @te_loadString()
       obj:loadFile     := @te_loadFile()
       obj:reLoadFile   := @te_reLoadFile()
       obj:saveFile     := @te_saveFile()
       obj:saveString   := @te_saveString()

       obj:loadBlock    := @te_loadBlock()
       obj:saveBlock    := @te_saveBlock()
       obj:beginBlock   := @te_beginBlock()
       obj:endBlock     := @te_endBlock()
       obj:cancelBlock  := @te_cancelBlock()
       obj:copyBlock    := @te_copyBlock()
       obj:moveBlock    := @te_moveBlock()
       obj:deleteBlock  := @te_deleteBlock()
       obj:setTabSize   := @te_setTabSize()
       obj:setFocus     := @te_setFocus()

       obj:copyToClipboard      := @te_copyToClipboard()
       obj:addToClipboard       := @te_addToClipboard()
       obj:moveToClipboard      := @te_moveToClipboard()
       obj:pasteFromClipboard   := @te_pasteFromClipboard()

       obj:refresh      := @te_refresh()
       //obj:refreshStr   := @te_refreshStr()

       obj:backSpace    := @te_backSpace()
       obj:delLeft      := @te_backSpace()
       obj:delHome      := @te_delHome()
       obj:delRight     := @te_delRight()
       obj:delete       := @te_delRight()
       obj:deleteLine   := @te_deleteLine()
       obj:delEnd       := @te_delEnd()
       //obj:delWordLeft  := @te_delWordLeft()
       //obj:delWordRight := @te_delWordRight()

       obj:Insert       := @te_Insert()
       obj:overStrike   := @te_overStrike()
       obj:__autoWrap   := @te___autoWrap()
       obj:insertLine   := @te_insertLine()
       obj:newLine      := @te_newLine()

       obj:draw         := @te_draw()
       obj:__Frmt       := @te_Frmt()
       obj:formatLine   := @te_formatLine()
       obj:formatPart   := @te_formatPart()
       obj:centerLine   := @te_centerLine()
       obj:insTempl     := @te_insTempl()
       obj:insMacro     := @te_insMacro()

       obj:__check_line := @te_check_line()

       obj:undo         := @te_undo()
       obj:writeundo    := @te_writeundo()

       obj:print        := @te_print()
       obj:printBlock   := @te_printBlock()

       obj:__around_check       := @te_around_check()
       obj:highLightAdd := @te_highLightAdd()
       obj:highLightDel := @te_highLightDel()

       obj:setNumStyle  := @te_setNumStyle()
       obj:killNumStyle := @te_killNumStyle()

       obj:setNewColor  := @te_setNewColor()
       obj:__setColor   := @__te_setcolor()

       obj:setCharset   := @te_setCharset()
       obj:__check_clipchs := @te_check_clipchs()
       obj:__setDefaultKey      := @te___setDefaultKey()
       obj:setKey       := @te_setKey()
       obj:applyKey     := @te_applyKey()
return obj
