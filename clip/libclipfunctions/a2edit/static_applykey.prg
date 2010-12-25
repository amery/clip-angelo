STATIC PROCEDURE ApplyKey(browse, nKey)
    local status
    DO CASE
    CASE nKey == K_DOWN
	browse:down()
    CASE nKey == K_PGDN
	browse:pageDown()
    CASE nKey == K_CTRL_PGDN
	browse:goBottom()
    CASE nKey == K_UP
	browse:up()
    CASE nKey == K_PGUP
	browse:pageUp()
    CASE nKey == K_CTRL_PGUP
	browse:goTop()
    CASE nKey == K_RIGHT
	browse:right()
    CASE nKey == K_LEFT
	browse:left()
    CASE nKey == K_HOME
	browse:home()
    CASE nKey == K_END
	browse:end()
    CASE nKey == K_CTRL_LEFT
	browse:panLeft()
    CASE nKey == K_CTRL_RIGHT
	browse:panRight()
    CASE nKey == K_CTRL_HOME
	browse:panHome()
    CASE nKey == K_CTRL_END
	browse:panEnd()
    OTHERWISE
    ENDCASE
RETURN
