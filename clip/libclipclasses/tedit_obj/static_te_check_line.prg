static function te_check_line(line, undo)
local i:=len(::edbuffer), oldLine
   undo := iif(undo==NIL, .t., undo)
   if line<=0
      return 1
   endif

/*
   if undo
	::writeundo(HASH_CHECKLINE)
   endif
*/
   oldLine := ::line
   for i=i to line-1
       aadd(::edbuffer,"")
	if undo
		::line := i+1
	    //  ::pos := 1
		::writeundo(HASH_INSLINE)
	endif
   next
  ::lines:=len(::edbuffer)
return oldLine
