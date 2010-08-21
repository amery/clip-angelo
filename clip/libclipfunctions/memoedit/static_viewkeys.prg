static func viewKeys(nkey,tobj,lExit)
    local ln,pos,oldcolor, ret:=.t.
    lExit:=.F.
    do case
       case (nkey==K_ESC .OR. nkey=K_CTRL_W) .AND. !tobj:updated
	    lExit:=.T.
       case nkey==K_DOWN
	    tobj:down()
       case nkey==K_PGDN
	    tobj:pageDown()
       case nkey==K_CTRL_PGDN
	    tobj:Bottom()
       case nkey==K_UP
	    tobj:up()
       case nkey==K_PGUP
	    tobj:pageUp()
       case nkey==K_CTRL_PGUP
	    tobj:top()
       case nkey==K_HOME
	    tobj:home()
       case nkey==K_END
	    tobj:end()
       case nkey==K_LEFT
	    tobj:left()
       case nkey==K_RIGHT
	    tobj:right()
       case nkey==K_CTRL_LEFT
	    tobj:wordLeft()
       case nkey==K_CTRL_RIGHT
	    tobj:wordRight()
       /*
       case nkey==K_F4
	    oldcolor=setcolor(set("edit_colors_menu"))
	    ln:=val(str(tobj:line,6,0))
	    pos:=val(str(tobj:pos,3,0))
	    ln=readkeyb(,,[Goto line],@ln,[Goto position],@pos)
	    tobj:gotoLine(ln)
	    tobj:gotoPos(pos)
	    setcolor(oldcolor)

       case nkey==K_ALT_F4
	    oldcolor=setcolor(set("edit_colors_menu"))
	    ln:=val(str(tobj:marginLeft,6,0))
	    pos:=val(str(tobj:marginRight,3,0))
	    ln=readkeyb(,,[Margin left],@ln,[Margin Right],@pos)
	    tobj:marginLeft:=ln
	    tobj:marginRight:=pos
	    setcolor(oldcolor)
       */
       otherwise
	    ret:=.f.
    endcase
return ret
