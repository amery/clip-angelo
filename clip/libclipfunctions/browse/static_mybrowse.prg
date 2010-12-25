STATIC PROCEDURE MyBrowse(nTop, nLeft, nBottom, nRight)

    LOCAL browse                        // The TBrowse object
    LOCAL cColorSave, nCursSave         // State preservers
    LOCAL nKey                          // Keystroke
    LOCAL lMore                         // Loop control
    local strMsg1:=[<bof>]
    local strMsg2:=[<new>]


    browse := StockBrowseNew(nTop, nLeft, nBottom, nRight)
    TURN_OFF_APPEND_MODE(browse)
    //browse:skipBlock := { |x| local(_b:=@browse), Skipper(x, _b) }
    browse:skipBlock := { |x| Skipper(x, browse) }
    browse:headSep := MY_HEADSEP
    browse:colSep := MY_COLSEP
    FancyColors(browse)
    nCursSave := SetCursor(SC_NONE)
    lMore := .T.
    DO WHILE lMore

	nKey := 0
	DO WHILE ! browse:stable //.and. nextKey() == 0
	    browse:stabilize()
	ENDDO
	while dispcount()>0
		dispend()
	enddo
	dispOutAt(ntop-1, nright-40, MSG_RECORDS+padr(alltrim(str(recno()))+"/"+alltrim(str(lastrec())),20) )
	//dispOutAt( ntop, nright-40, iif(deleted(),[Deleted],[Actived]) )
	IF browse:stable

	    IF browse:hitBottom .AND. .NOT. IS_APPEND_MODE(browse)
		TURN_ON_APPEND_MODE(browse)
		dispOutAt( ntop-1, nright-10, strMsg2 )
		nKey := K_DOWN
	    ELSE
		IF browse:hitTop .OR. browse:hitBottom
		    dispOutAt( ntop-1, nright-10, iif(bof(),strMsg1,strMsg2) )
		    TONE(125, 0)
		else
			if !IS_APPEND_MODE(browse)
				dispOutAt( ntop-1, nright-10, space(max(len(strMsg1),len(strMsg2))) )
			endif
		ENDIF
	    ENDIF
	    if nkey==0
		browse:refreshCurrent()
		browse:ForceStable()
		nKey := InKey(0)
		if setkey(nKey)!=NIL
			eval(setkey(nKey),procname(),procline(),readvar())
			loop
		endif
	    endif
	ENDIF

	IF nKey == K_ESC
	    lMore := .F.
	ELSE
	    ApplyKey(browse, nKey)
	ENDIF
    ENDDO
    SETCURSOR(nCursSave)
RETURN
