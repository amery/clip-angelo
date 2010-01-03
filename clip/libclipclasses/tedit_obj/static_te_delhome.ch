static function te_delHome(undo)
   undo := iif(undo==NIL, .t., undo)
   ::__check_line(::line)
   if undo
	::writeundo(HASH_DELHOME, substr(::edbuffer[::line], 1, ::pos-1))
   endif
   ::edbuffer[::line]=substr(::edbuffer[::line],::pos)
   ::updated:=.t.
   ::pos := 1
   ::colWin := 1
   if undo
	::refresh()
   endif
RETURN
