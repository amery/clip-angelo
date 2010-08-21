function iMenuBegin( lPopup, lSystem, oWnd )
	local oItem


	if len(aMenuStack)==0
        	oItem := topBar(0,0,maxcol() )
                aadd(aMenuStack,oItem)
        else
                oItem := popUp()
                aadd(aMenuStack,oItem )
        	oLastItem:data := oItem
        endif

return oItem
