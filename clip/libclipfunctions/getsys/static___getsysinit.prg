static function __getSysInit()
   if oStatus==NIL
	oStatus=map()
	oStatus:updated     := .f.
	oStatus:format      := NIL
	oStatus:killread    := NIL
	oStatus:bumptop     := NIL
	oStatus:bumpbot     := NIL
	oStatus:lastexit    := NIL
	oStatus:lastpos     := NIL
	oStatus:activeget   := NIL
	oStatus:readvar     := NIL
	oStatus:procname    := NIL
	oStatus:procline    := NIL
	oStatus:nextget     := NIL
	oStatus:hitcode     := NIL
	oStatus:pos         := NIL
	oStatus:scrvmsg     := NIL
	oStatus:menuid      := NIL
	oStatus:svccursor   := NIL
   endif
return
