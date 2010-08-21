STATIC PROCEDURE ApplyKey(browse, nKey)
	local bBlock
    DO CASE
    CASE nKey == K_F8
	iif(deleted(),dbrecall(),dbdelete())
    CASE nKey == K_CTRL_PGDN
	browse:goBottom()
	TURN_OFF_APPEND_MODE(browse)
    CASE nKey == K_UP
	browse:up()
	IF IS_APPEND_MODE(browse)
	    TURN_OFF_APPEND_MODE(browse)
	    browse:refreshAll()
	ENDIF
    CASE nKey == K_PGUP
	browse:pageUp()
	IF IS_APPEND_MODE(browse)
	    TURN_OFF_APPEND_MODE(browse)
	    browse:refreshAll()
	ENDIF
    CASE nKey == K_CTRL_PGUP
	browse:goTop()
	TURN_OFF_APPEND_MODE(browse)
    CASE nKey == K_RETURN
	DoGet(browse)
    OTHERWISE
	bBlock := browse:setkey(nkey)
	if bBlock != NIL
		eval(bBlock, browse, nkey)
	else
		if nKey >=32 .and. nKey < 1000
			KEYBOARD CHR(nKey)
			DoGet(browse)
		endif
	endif
    ENDCASE
RETURN
