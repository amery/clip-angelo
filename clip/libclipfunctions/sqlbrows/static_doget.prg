STATIC PROCEDURE DoGet(browse)
	LOCAL bIns, lScore, lExit, pic
	LOCAL col, get, nKey

	browse:ForceStable()
	lScore := Set(_SET_SCOREBOARD, .F.)
	lExit := Set(_SET_EXIT, .T.)
	bIns := SetKey(K_INS)
	SetKey( K_INS, {|| InsToggle()} )
	SetCursor( IF(ReadInsert(), SC_INSERT, SC_NORMAL) )
	col := browse:getColumn(browse:colPos)
	pic:="@S"+alltrim(str(browse:nRight-col()+1))
	if eval(col:block) == NIL
		switch(col:type)
			case 'C'
				eval(col:block,space(col:_len))
			case 'N'
				eval(col:block,0)
			case 'D'
				eval(col:block,stod("        "))
			case 'L'
				eval(col:block,.F.)
// No GET DATETIME_t implemented yet
//			case 'T'
//				eval(col:block,STOT())
		endswitch
	endif
	get := GetNew(Row(), Col(), col:block, col:heading, pic, browse:colorSpec)
	ReadModal( {get} )
	SetCursor(0)
	Set(_SET_SCOREBOARD, lScore)
	Set(_SET_EXIT, lExit)
	SetKey(K_INS, bIns)

	browse:refreshAll()
	browse:ForceStable()

	nKey := LASTKEY()
	IF nKey == K_UP .OR. nKey == K_DOWN .OR. ;
		nKey == K_PGUP .OR. nKey == K_PGDN
		KEYBOARD( CHR(nKey) )
	ENDIF
RETURN
