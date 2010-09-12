static function te_deleteLine(undo)
local min, max
   undo := iif(undo==NIL, .t., undo)
   ::__check_line(::line)
   if undo
	::writeundo(HASH_DELINE, ::edbuffer[::line])
   endif
   adel(::edbuffer,::line)
   asize(::edBuffer,len(::edBuffer)-1)
   ::lines:=len(::edbuffer)
   ::updated:=.t.
   if ::strblock .or. ::rectblock
	max := max(::koordblock[1], ::koordblock[3])
	min := min(::koordblock[1], ::koordblock[3])
	if ::line < min
		::koordblock[1] := min-1
		::koordblock[3] := max-1
	else
		if between(::line, min, max)
			::koordblock[1] := min
			::koordblock[3] := max-1
			if ::koordblock[1] > ::koordblock[3]
				::cancelblock(undo)
			endif
		endif
	endif
   endif
   if undo
	::refresh()
   endif
return
