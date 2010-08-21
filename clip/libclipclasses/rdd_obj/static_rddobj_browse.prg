static function rddobj_browse(self,top, left, down, right, fields, user_func, picts, headers, hsep,csep,fsep, footers)

    LOCAL bSaveHandler, error, status
    LOCAL browser                       // The TBrowse object
    LOCAL cColorSave, nCursSave         // State preservers
    LOCAL nKey                          // Keystroke
    LOCAL lMore                         // Loop control
    LOCAL cScreen, __user_func,ret

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

    browser := TBrowseDB(Top, Left, Down, Right)
    self:browse_StockBrowseNew(browser, fields, headers, footers, picts, hsep, csep, fsep )
    browser:skipBlock := { |x| self:browse_skipper(x, browser) }
    browser:headSep := MY_HEADSEP
    browser:colSep := MY_COLSEP
    self:browse_FancyColors(browser)
    lMore := .T.
    DO WHILE lMore
        IF browser:colPos <= browser:freeze
            browser:colPos := browser:freeze + 1
        ENDIF

        nKey := 0
        DO WHILE nKey == 0 .AND. .NOT. browser:stable
            browser:stabilize()
            nKey := InKey()
        ENDDO

        IF browser:stable
                IF browser:hitTop .OR. browser:hitBottom
                    TONE(125, 0)
                ENDIF
                browser:refreshCurrent()
                browser:ForceStable()
                nKey := InKey(0)
        ENDIF

        IF empty(user_func) .and. (nKey == K_ESC .or. nkey==K_ENTER)
            lMore := .F.
        ELSE
            ApplyKey(browser, nKey)
            if empty(user_func)
               loop
            endif
            status:=DE_IDLE
            if self:lastrec()==0
               status:=DE_EMPTY
            endif
            if browser:hitTop
               status:=DE_HITTOP
            endif
            if browser:hitBottom
               status:=DE_HITBOTTOM
            endif
            // DE_EXCEPT           4          // ������� �� ��������������
            if "(" $ user_func
                __user_func:=user_func
            else
                __user_func:=user_func+"("+alltrim(str(status))+","+alltrim(str(browser:colPos))+")"
            endif
            ret=&__user_func
            if ret==DE_ABORT
               lmore:=.f.
            endif
            if ret==DE_REFRESH
               browser:refreshAll()
            endif
        ENDIF
    ENDDO

    SETCURSOR(nCursSave)
    RESTSCREEN(,,,,cScreen)

return  NIL
