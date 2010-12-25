function fx_wActivate(cName)
	local wNo,oWin, bType
        local x1,x2,s1,s2
	wNo:= fx_winNum(cName)
        if wNo == 0
        	return
        endif
	oWin:=fx_windows[wNo][2]
        do case
        	case oWin:system
                	bType:=1  //WB_11
                case oWin:double
                	bType:=2 //WB_0
                case oWin:panel
                	bType:=1 //WB_11
                case oWin:none
                	bType:=""
                otherwise
                	bType:=1 //WB_1
        endcase
        setcolor(oWin:color)
        dispBegin()
        set cursor on
        if oWin:winNo == NIL
        	oWin:winNo := wopen(oWin:top,oWin:left,oWin:bottom,oWin:right,.t.)
                wselect(oWin:winNo)
                if !empty(bType)
                	wbox(bType)
                endif
                clear screen
                x2:=oWin:right-oWin:left-2
               	x1:=(x2-len(oWin:title))/2
                x1:=max(1,x1)
                if !oWin:none .and. !empty(oWin:title)
                        s1:=alltrim(padr(oWin:title,x2))
                        @ -1, x1 say s1
                endif
                if !oWin:none .and. !empty(oWin:footer)
                        s1:=alltrim(padr(oWin:footer,x2))
                        //@ oWin:bottom+1, x1 say s1
                endif
                aadd(fx_wstack,oWin:winNo)
        endif
        dispEnd()
return
