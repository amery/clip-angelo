function iMenuAddItem( cPrompt, cMsg, lChecked, lActive, bAction, cBmpFile,;
                      cResName, osMenu, bBlock, nVKState, nVirtKey, lHelp,;
                      nHelpId, bWhen, lBreak, nEvent )

	local oItem

	if pcount() == 0
      		oItem := menuItem( MENU_SEPARATOR )
        else
        	if len(aMenuStack)==1
        		oItem := menuItem ( cPrompt, popUp() )
		else
        		oItem := menuItem ( cPrompt )
                        if lChecked != NIL
        			oItem:checked := lChecked
                        endif
                        if lActive != NIL
        			oItem:enabled := lActive
                        endif
        		if cMsg != NIL
        			oItem:message := cMsg
        		endif
        		if nVirtKey != NIL
	        		oItem:shortCut := nVirtKey
	        	endif
	        	if bBlock != NIL
	        		oItem:data := bBlock
	        	endif
	        	if bAction != NIL
	        		oItem:data := bAction
	        	endif
	        	if nEvent != NIL
	        		oItem:id := nEvent
	        	endif
		endif
	endif
	oLastItem:=oItem
	aMenuStack[len(aMenuStack)]:addItem(oItem)

return oItem
