static function te_goTop( undo )
   undo := iif(undo==NIL, .t., undo)
   if undo
	::writeundo(HASH_BOTOP)
   endif
   ::line:=1
   ::rowWin:=1
   if undo
	::refresh()
   endif
RETURN
