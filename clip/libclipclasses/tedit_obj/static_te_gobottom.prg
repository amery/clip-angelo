static function te_goBottom( undo )
   undo := iif(undo==NIL, .t., undo)
   if undo
	::writeundo(HASH_BOTOP)
   endif
   ::line:=::lines+1
   ::rowWin:=::nbottom-::ntop
   if undo
	::refresh()
   endif
RETURN
