STATIC FUNCTION ClearGetSysVars()
   LOCAL k, aSavSysVars  := map()

   for k in oStatus KEYS
	 aSavSysVars[k] := oStatus[k]
   next
   aSavSysVars:updated   := .F.
   aSavSysVars:readvar   := READVAR( "" )
   aSavSysVars:activeget := GETACTIVE( NIL )

   oStatus:updated       := .F.
   oStatus:format        := NIL
   oStatus:killread      := .F.
   oStatus:bumptop       := .F.
   oStatus:bumpbot       := .F.
   oStatus:lastexit      := 0
   oStatus:lastpos       := 0
   oStatus:procname  := ""
   oStatus:procline  := 0
   oStatus:nextget       := 0
   oStatus:hitcode       := 0
   oStatus:pos           := 0
   oStatus:scrvmsg      := NIL

   RETURN ( aSavSysVars )
