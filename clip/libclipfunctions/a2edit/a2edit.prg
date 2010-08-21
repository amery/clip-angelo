function a2edit

    LOCAL bSaveHandler, error, status
    LOCAL browse                        // The TBrowse object
    LOCAL cColorSave, nCursSave         // State preservers
    LOCAL nKey                          // Keystroke
    LOCAL lMore                         // Loop control
    LOCAL cScreen,ret
    private __userfunc, curr_record:=1

    parameters  top, left, down, right, __a2_data, user_func, msay, headers, none1,non2,none3, footers

    if len(__a2_data)==0 .or. len(__a2_data[1])==0
       alert([Error A2EDIT; What~s Edit ?])
       return
    endif
    top  := iif( top  == NIL, 0, top )
    left := iif( left == NIL, 0, left )
    down  := iif( down  == NIL, maxrow(), down )
    right:= iif( right== NIL, maxcol() , right )
    top=max(top,0)
    left=max(left,0)
    down=min(down,maxrow())
    right=min(right,maxcol())

    cScreen := SAVESCREEN()
    nCursSave := SetCursor(SC_NONE)

    browse := TBrowseDB(Top, Left, Down, Right)
    StockBrowseNew(browse, len(__a2_data), headers, footers )
    browse:skipBlock := { |x| Skipper(x, browse) }
    browse:headSep := MY_HEADSEP
    browse:colSep := MY_COLSEP
    FancyColors(browse)
    lMore := .T.
    DO WHILE lMore
	IF browse:colPos <= browse:freeze
	    browse:colPos := browse:freeze + 1
	ENDIF

	nKey := 0
	DO WHILE nKey == 0 .AND. .NOT. browse:stable
	    browse:stabilize()
	    nKey := InKey()
	ENDDO

	IF browse:stable
		IF browse:hitTop .OR. browse:hitBottom
		    TONE(125, 0)
		ENDIF
		browse:refreshCurrent()
		browse:ForceStable()
		nKey := InKey(0)
	ENDIF

	IF empty(user_func) .and. (nKey == K_ESC .or. nkey==K_ENTER)
	    lMore := .F.
	ELSE
	    ApplyKey(browse, nKey)

	    if empty(user_func)
	       loop
	    endif
	    status:=DE_IDLE
	    if lastrec()==0
	       status:=DE_EMPTY
	    endif
	    if browse:hitTop
	       status:=DE_HITTOP
	    endif
	    if browse:hitBottom
	       status:=DE_HITBOTTOM
	    endif
	    // DE_EXCEPT           4          // ������� �� �������������� DBEDIT()
	    if "(" $ user_func
		__userfunc:=user_func
	    else
		__userfunc:=user_func+"("+alltrim(str(status))+","+alltrim(str(browse:colPos))+")"
	    endif
	    ret=&__userfunc
	    if ret==DE_ABORT
	       lmore:=.f.
	    endif
	    if ret==DE_REFRESH
	       browse:refreshAll()
	    endif
	ENDIF
    ENDDO

    SETCURSOR(nCursSave)
    RESTSCREEN(,,,,cScreen)

return  NIL
