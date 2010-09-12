static function te_mgoto(nRow, nCol, undo)
local len

  if nRow==NIL .or. nRow < 1
	return .f.
  endif

  if nCol==NIL .or. nCol < 1
	return .f.
  endif

   undo := iif(undo==NIL, .t., undo)
   if undo
	::writeundo(HASH_MGOTO)
   endif
   if ::line>::lines
	::line := ::lines
   endif
   len := len(::edbuffer[::line])
   ::pos += nCol - ::colWin
   ::colWin:=nCol
   ::line += nRow - ::rowWin
   ::rowWin:=nRow
   if undo
	::refresh()
   endif
return .t.
