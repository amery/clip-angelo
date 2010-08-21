*reference fx_winNum
function fx_wDefine(cName,x1,y1,x2,y2,cTitle,cFoot,;
			system,double,panel,none,;
			shadow,zoom,color,all)
	local wNo, w:= map()
	wNo:= fx_winNum(cName)
        if wNo != 0
        	w :=@ fx_windows[wNo]
                w := map()
        endif
        cname := upper(cname)
        w:classname	:= "FX_WINDOW"
        w:name		:= cName
        w:winNo		:= NIL
        w:color		:= iif(empty(color),setcolor(),color)
        w:top		:= iif(x1==NIL,1,x1)
        w:left		:= iif(y1==NIL,0,y1)
        w:bottom	:= iif(x2==NIL,maxrow(.t.),w:top+x2+1)
        w:right		:= iif(y2==NIL,maxcol(.t.),w:left+y2+1)
        w:title		:= alltrim(cTitle)
        w:footer	:= alltrim(cFoot)
        w:system	:= system
        w:double	:= double
        w:panel		:= panel
        w:none		:= none
        w:shadow	:= shadow
        w:zoom		:= zoom
        aadd(fx_windows,{cName,w})
return
