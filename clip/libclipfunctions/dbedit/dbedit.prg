function dbedit (top, left, down, right,;
		 fields, user_func,;
		 msay, headers, headSep,;
		 colSep,footSep, footers)

    LOCAL bSaveHandler, error, status
    LOCAL browse                        // The TBrowse object
    LOCAL cColorSave, nCursSave         // State preservers
    LOCAL nKey                          // Keystroke
    LOCAL lMore                         // Loop control
    LOCAL cScreen,filter
    local row,col,ret
    local first:=.t.

#ifdef DE_APPEND_SUPPORT
    private __de_append_mode := .f.
#endif


    user_func := iif( valtype(user_func) $ "CB", user_func, NIL)
    top  := iif( top  == NIL, 0, top )
    left := iif( left == NIL, 0, left )
    down  := iif( down  == NIL, maxrow(), down )
    right:= iif( right== NIL, maxcol() , right )
    top=max(top,0)
    left=max(left,0)
    down=min(down,maxrow())
    right=min(right,maxcol())

//    cScreen := SAVESCREEN(top, left, down, right)
    nCursSave := SetCursor(SC_NONE)

    browse := TBrowseDB(Top, Left, Down, Right)
    browse:setkey(K_ESC,NIL)
    StockBrowseNew(browse, fields, headers, footers, msay, headSep, colSep, footSep )
    browse:skipBlock := { |x|  Skipper(x, browse) }
    browse:headSep := DE_HEADSEP
    browse:colSep := DE_COLSEP
    FancyColors(browse)

    browse:forceStable()

    lMore := .T.
    DO WHILE lMore
	IF browse:colPos <= browse:freeze
	    browse:colPos := browse:freeze + 1
	ENDIF

	DO WHILE !browse:stable .and. nextkey()==0
	    browse:stabilize()
	ENDDO

	nKey := InKey()
	ret:=DE_CONT

	if nkey==0 .or. first
		//row:=row(); col:=col()
		ret=callUserFunc(browse,user_func,0)
		//dispoutAt(row,col,"")

		if ret==DE_ABORT
			lmore:=.f.
			loop
		endif
		if ret==DE_REFRESH
			browse:refreshAll()
			//loop
		endif
		first:=.f.
		if nkey==0
			nKey := InKey(0)
		endif
	endif

	if setKey(nKey)!=NIL
		eval(setKey(nKey),procname(),procline(),readvar())
		loop
	endif

	IF empty(user_func) .and. (nKey == K_ESC .or. nkey==K_ENTER)
	    lMore := .F.
	    loop
	endif
	filter:=dbfilter()
	if !ApplyKey(browse, nKey)
		//row:=row(); col:=col()
		ret=callUserFunc(browse,user_func,nkey)
		first:=.t.
		//dispoutAt(row,col,"")
	endif
	if ret==DE_ABORT
		lmore:=.f.
	endif
	if filter != dbfilter() .and. used()
		ret=DE_REFRESH
	endif
	if ret==DE_REFRESH
		browse:refreshAll()
	endif
    ENDDO
/*
    if used()
	browse:forcestable()
    endif
*/

    SETCURSOR(nCursSave)
//    RESTSCREEN(top, left, down, right,cScreen)

return  NIL
