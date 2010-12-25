STATIC PROCEDURE DoGet(browse)
    LOCAL bIns, lScore, lExit, pic
    LOCAL col, get, nKey
    LOCAL lAppend:=.f., xOldKey, xNewKey
    browse:ForceStable()
    lAppend := IS_APPEND_MODE(browse)
    IF lAppend .AND. RECNO() == RECCOUNT() + 1
	APPEND BLANK
    ENDIF
    //xOldKey := IF( EMPTY(INDEXKEY()), NIL, &(INDEXKEY()) )
    xOldKey := NIL
    lScore := Set(_SET_SCOREBOARD, .F.)
    lExit := Set(_SET_EXIT, .T.)
    bIns := SetKey(K_INS)
    SetKey( K_INS, {|| InsToggle()} )
    SetCursor( IF(ReadInsert(), SC_INSERT, SC_NORMAL) )
    col := browse:getColumn(browse:colPos)
    pic:="@S"+alltrim(str(browse:nRight-col()+1))
    get := GetNew(Row(), Col(), col:block, col:heading, pic, browse:colorSpec)
    ReadModal( {get} )
    SetCursor(0)
    Set(_SET_SCOREBOARD, lScore)
    Set(_SET_EXIT, lExit)
    SetKey(K_INS, bIns)
    //xNewKey := IF( EMPTY(INDEXKEY()), NIL, &(INDEXKEY()) )
    xNewKey := NIL
    IF .NOT. (xNewKey == xOldKey) .OR. (lAppend .AND. xNewKey != NIL)
	browse:refreshAll()
	browse:ForceStable()
	DO WHILE &(INDEXKEY()) > xNewKey .AND. .NOT. browse:hitTop()
	    browse:up()
	    browse:ForceStable()
	ENDDO
    ENDIF
    TURN_OFF_APPEND_MODE(browse)
    nKey := LASTKEY()
    IF nKey == K_UP .OR. nKey == K_DOWN .OR. ;
	nKey == K_PGUP .OR. nKey == K_PGDN
	KEYBOARD( CHR(nKey) )
    ENDIF
RETURN
