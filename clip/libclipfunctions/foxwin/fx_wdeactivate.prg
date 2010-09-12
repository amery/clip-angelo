function fx_wDeactivate(cName)
	local wNo,oWin
	wNo:= fx_winNum(cName)
        if wNo == 0
        	return
        endif
	oWin:=fx_windows[wNo][2]
        dispBegin()
        if oWin:winNo != NIL
        	wselect(oWin:winNo)
                wclose()
                oWin:winNo := NIL
                if len(fx_wstack) > 1
                        asize(fx_wstack, len(fx_wstack)-1 )
                endif
                if len(fx_wstack) > 0
                	wselect( atail( fx_wstack ) )
                endif
        endif
        dispEnd()
return
