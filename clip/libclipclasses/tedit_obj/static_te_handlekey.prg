static function te_handleKey(nkey)
local ret:=.t.
    do case
       case nkey==K_DOWN
	    ::down()
       case nkey==K_PGDN
	    ::pageDown()
       case nkey==HASH_BottomText//K_CTRL_PGDN
	    ::Bottom()
       case nkey==K_UP
	    ::up()
       case nkey==K_PGUP
	    ::pageUp()
       case nkey==HASH_TopText//K_CTRL_PGUP
	    ::Top()
       case nkey==K_HOME
	    ::home()
       case nkey==K_END
	    ::end()
       case nkey==K_LEFT
	    ::left()
       case nkey==K_RIGHT
	    ::right()
       case nkey==K_CTRL_LEFT
	    ::wordLeft()
       case nkey==K_CTRL_RIGHT
	    ::wordRight()
       otherwise
	    ret:=.f.
    endcase
return ret
