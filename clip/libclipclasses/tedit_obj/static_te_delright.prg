static function te_delRight( undo )
local min
   if ::line > ::lines
	return
   endif
   undo := iif(undo==NIL, .t., undo)
   ::__check_line(::line)
   if ::pos<=len(::edbuffer[::line])
       if undo
	   ::writeundo(HASH_DEL, substr(::edbuffer[::line], ::pos, 1))
       endif
       ::edbuffer[::line]=substr(::edbuffer[::line],1,::pos-1)+substr(::edbuffer[::line],::pos+1)
   else
       if undo
	   ::writeundo(HASH_DEL, chr(K_ENTER))
       endif
       if ::line+1 <= ::lines
		::edbuffer[::line]=::edbuffer[::line]+::edbuffer[::line+1]
		adel(::edbuffer, ::line+1)
		::lines--
		asize(::edbuffer, ::lines)
       elseif ::line<=::lines .and. (len(::edbuffer[::line])==0)
		adel(::edbuffer, ::line)
		::lines--
		asize(::edbuffer, ::lines)
       endif
   endif
   if ::rectblock .and. ::line == ::koordblock[1] .and. ::line == ::koordblock[3]
     min := min(::koordblock[2], ::koordblock[4])
     if ::pos < min
	::koordblock[2] --
	::koordblock[4] --
     else
	if between(::pos, ::koordblock[2], ::koordblock[4])
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
