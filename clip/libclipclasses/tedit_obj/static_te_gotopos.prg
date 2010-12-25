static function te_gotoPos(pos, cw, undo)
local len

  if pos==NIL .or. pos < 1
	return .f.
  endif

   undo := iif(undo==NIL, .t., undo)
   if undo
	::writeundo(HASH_GOPOS)
   endif
   if ::line>::lines
	::line := ::lines
   endif
   len := len(::edbuffer[::line])
   /*
   if pos > len
	pos := len
   endif
   */
   ::pos:=pos
   if cw == NIL
	::colWin:=pos
   else
	::colWin:=cw
   endif
   if undo
	::refresh()
   endif
return .t.
