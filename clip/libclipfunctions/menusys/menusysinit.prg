procedure menusysInit()
   if oStatus==NIL
	oStatus:=map()
	oStatus:menuList     := NIL
	oStatus:menuLevel    := NIL
	oStatus:oMenu        := NIL
	oStatus:oldMessage   := NIL
	oStatus:scrVmsg      := NIL
	oStatus:oldMsgFlag   := NIL
	oStatus:oldRow       := row()
	oStatus:oldCol       := col()
	oStatus:oldCursor    := SC_NORMAL
   endif
return
