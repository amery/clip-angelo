static function te_delEnd(undo)
   undo := iif(undo==NIL, .t., undo)
   ::__check_line(::line)
   if undo
	::writeundo(HASH_DELEND, substr(::edbuffer[::line], ::pos))
   endif
   ::edbuffer[::line]=substr(::edbuffer[::line],1,::pos-1)
   ::updated:=.t.
   if undo
	::refresh()
   endif
RETURN
