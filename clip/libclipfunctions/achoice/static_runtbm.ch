static func runTBm(bb)
local vyb,i,nnn
local scr,curCol,CurRow
local retVal:=0,mode:=2
local nkey:=0, mKey:=0, lMore:=.t.
local nskip,xskip,oldlen
local bKeyBlock, scrTop:=savescreen(__ac_data:nT,__ac_data:nL,__ac_data:nT,__ac_data:nR)
local ufunc:=__ac_data:FunName
memvar __ac_data

    lMore := .t.
    scr:=savescreen(__ac_data:nT+1,__ac_data:nR,__ac_data:nT+1,__ac_data:nR)
    while ( lmore )

       //dispbegin()
       while ( !bb:stabilize() )
       enddo
       //dispend()

       mode:=4
       if __ac_data:exit
	   bb:dehilite()
	   oldLen := __ac_data:lenMass
	   if valtype(uFunc) == "C" .and. __ac_data:flagUserF
	      mode:=&uFunc.(mode,__ac_data:nRowMass,bb:rowpos-1,bb)
	   elseif valtype(uFunc) == "B" .and. __ac_data:flagUserF
	      mode:=eval(uFunc,mode,__ac_data:nRowMass,bb:rowpos-1, bb)
	   endif
	   __ac_data:lenMass:=min(__ac_data:lenMass,len(__ac_data:massp))
	   if oldLen != __ac_data:lenMass
	     bb:refreshAll()
	   endif
	   return 0
       endif
       for i=1 to __ac_data:lenMass
	   if __ach_color_block(i)
	      mode:=3
	      exit
	   endif
       next
       if  mode==4
	    lmore:=.f.
	    retVal:=0
       elseif !__ach_color_block(__ac_data:nRowMass)
	   if __ac_data:nRowMass==1
		 keyBoard chr(K_DOWN)
	   elseif __ac_data:nRowMass==__ac_data:lenMass
	       keyBoard chr(K_UP)
	   elseif lastkey()==K_ENTER .or. lastkey()==K_ESC
		 keyBoard chr(K_UP)
	   else
	       keyBoard chr(lastkey())
	   endif
       endif
       curCol:=col()
       curRow:=row()

       if ( bb:stable .and. mode!=4 )
	  if __ac_data:boxFlag==.t.
		restscreen(__ac_data:nT,__ac_data:nL,__ac_data:nT,__ac_data:nR,scrTop)
		NNN:=alltrim(str(__ac_data:nRowMass,4,0))+":"+alltrim(str(__ac_data:lenMass,4,0))
		dispOutAt( __ac_data:nT,__ac_data:nR-len(NNN), NNN  )
		if __ac_data:nRowMass!=1
		   dispOutAt( __ac_data:nT+1,__ac_data:nR, "*")
		else
		   restscreen(__ac_data:nT+1,__ac_data:nR,__ac_data:nT+1,__ac_data:nR,scr)
		endif
		if __ac_data:nRowMass!=__ac_data:lenMass
		   dispOutAt(__ac_data:nB-1,__ac_data:nR, "*" )
		else
		   restscreen(__ac_data:nB-1,__ac_data:nR,__ac_data:nB-1,__ac_data:nR,scr)
		endif
		//setPos(curRow, curCol)
		dispOutAt(curRow, curCol,"")
	   endif
	   mKey:=0
	   do while .t. //!__ac_data:exit
		   nKey := InKey(0)
		   bb:hitTop := bb:hitBottom := .f.
		   if ( (bKeyBlock:=setkey(nKey))!=NIL )
		      eval(bKeyBlock,procname(),procline(),readvar())
		   else
		      exit
		   endif
	   enddo

       endif

       do case
       case ( nKey == K_ENTER) .and. !__ac_data:flagUserF
	    bb:hitTop := bb:hitBottom := .f.
	    retVal:=__ac_data:nRowMass
	    mode:=0
	    lmore:=.f.
	    exit
       case ( nKey == K_ESC)  .and. !__ac_data:flagUserF
	    retVal:=0
	    mode:=0
	    lmore:=.f.
	    exit
       case ( nKey == K_LEFT .or. nkey==K_RIGHT )  .and. !__ac_data:flagUserF
	    retVal:=0
	    mode:=0
	    lmore:=.f.
	    exit
       case ( nKey == K_DOWN)
	    bb:down()
	    mode:=0
       case ( nKey == K_UP)
	    bb:up()
	    mode:=0
       case ( nKey == K_PGUP )
	    mode:=0
	    bb:hitTop := bb:hitBottom := .f.
	    xskip=0-(__ac_data:nB-__ac_data:nT-2)
	    nskip := eval(bb:skipBlock,xskip)
	    if nskip>xskip
		bb:rowPos:= max(1,bb:rowpos-nskip+xskip)
	    endif
	    bb:refreshAll()
	    if nskip == 0
		bb:hitTop := .t.
	    endif
       case ( nKey == K_PGDN )
	    mode:=0
	    bb:hitTop := bb:hitBottom := .f.
	    xskip := __ac_data:nB-__ac_data:nT-2
	    nskip := eval(bb:skipBlock,xskip+1)
	    nskip := eval(bb:skipBlock,0-nskip)
	    nskip := eval(bb:skipBlock,xskip)
	    if nskip<xskip
		bb:rowPos:=min(bb:rowpos+nskip-xskip,__ac_data:nRowMass)
	    endif
	    if __ac_data:nRowMass == len(__ac_data:massp)
		bb:rowPos := __ac_data:nB-__ac_data:nT-1
	    endif
	    bb:rowPos := max(1,bb:rowpos)
	    bb:refreshAll()
	    if nskip == 0
		bb:hitBottom := .t.
	    endif
       case ( nKey == K_CTRL_HOME ) .or. nKey==K_HOME .or. nKey==K_CTRL_PGUP
	    mode:=0
	    __ac_data:nRowMass:=1
	    bb:rowPos:=1
	    bb:refreshAll()
       case ( nKey == K_CTRL_END )  .or. nKey==K_END .or. nKey==K_CTRL_PGDN
	    mode:=0
	    bb:goBottom()
    //	    bb:refreshAll()
       case (nKey>32 .and. nKey<255) .and. !__ac_data:flagUserF
	    mode:=firstChr(nKey,bb)
	    do case
		   case mode==2
		       bb:refreshCurrent()
		   case mode==3
		       bb:refreshAll()
	    endcase
       endcase
       if valtype(uFunc) $ "CB" .and. __ac_data:flagUserF //.and. nextkey()==0
	    bb:forceStable()
	    bb:dehilite()
	    IF bb:hitTop
	       mode:=1
	    ENDIF
	    IF bb:hitBottom
	       mode:=2
	    ENDIF
	    oldLen := __ac_data:lenMass
	    if valtype(uFunc) == "C"
		mode:=&uFunc.(mode,__ac_data:nRowMass,bb:rowpos-1,bb)
	    else
		mode:=eval(__ac_data:FunName,mode,__ac_data:nRowMass,bb:rowpos-1, bb)
	    endif
	    __ac_data:lenMass:=min(__ac_data:lenMass,len(__ac_data:massp))
	    if oldLen != __ac_data:lenMass
	      bb:refreshall()
	    endif
	    if mode==0
	       retval:=0
	       lmore:=.f.
	    endif
	    if mode==3
		 mode:=FirstChr(lastkey(),bb)
	    endif
	    if mode==1
	       retval:=__ac_data:nRowMass
	       lmore:=.f.
	    endif
	    do case
	      case mode==2 .or. mode==4
		   bb:refreshCurrent()
	      case mode==5 .or. mode==3
		   bb:refreshAll()
	   endcase
       endif
    enddo
return retVal
