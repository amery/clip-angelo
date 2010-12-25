static function te_backSpace( undo )
local min
   undo := iif(undo==NIL, .t., undo)
   if ::line > ::lines
	return
   endif
   ::__check_line(::line)
   if undo
	if ::pos == 1
	    ::writeundo(HASH_BS, chr(K_ENTER))
	else
	    ::writeundo(HASH_BS, substr(::edbuffer[::line], ::pos-1, 1))
	endif
   endif
   if ::pos != 1
       ::edbuffer[::line]=substr(::edbuffer[::line],1,::pos-2)+substr(::edbuffer[::line],::pos)
       ::pos --
       ::colWin --
   else
       if ::line == 1
	   return
       endif
	if ::rectblock .and. ::line == ::koordblock[1] .and. ::line == ::koordblock[3]
		::cancelBlock(.f.)
	endif
       ::lines--
       ::colWin := len(::edbuffer[::line-1])+1
       ::edbuffer[::line-1] += ::edbuffer[::line]
       adel(::edbuffer, ::line)
       asize(::edbuffer, ::lines)
       ::line--
       ::pos := ::colWin
   endif
   if ::rectblock .and. ::line == ::koordblock[1] .and. ::line == ::koordblock[3]
     min := min(::koordblock[2], ::koordblock[4])
     if ::pos+1 <= min
	::koordblock[2] --
	::koordblock[4] --
     else
	  if between(::pos+1, ::koordblock[2], ::koordblock[4])
		::koordblock[4] := max(::koordblock[2], ::koordblock[4])-1
		::koordblock[2] := min
		if ::koordblock[2]>::koordblock[4]
			::cancelBlock(.f.)
		endif
	  endif
     endif
   endif
   ::updated:=.t.
   if undo
	::refresh()
   endif
RETURN
