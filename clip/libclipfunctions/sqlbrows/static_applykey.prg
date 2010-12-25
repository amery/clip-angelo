STATIC PROCEDURE ApplyKey(browse, nKey)
	local bBlock
	DO CASE
		CASE nKey == K_CTRL_PGDN
			browse:goBottom()
		CASE nKey == K_UP
			browse:up()
		CASE nKey == K_PGUP
			browse:pageUp()
		CASE nKey == K_CTRL_PGUP
			browse:goTop()
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
