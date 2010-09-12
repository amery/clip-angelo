static function te_gotoLine(ln, rw, undo)
  if ln==NIL .or. ln < 1 .or. ln > ::lines
	return .f.
  endif

  undo := iif(undo==NIL, .t., undo)

  if undo
	::writeundo(HASH_GOLINE)
  endif
  ::line := ln
  if rw == NIL
	::rowWin := int((::nBottom-::nTop)*2/3)
  else
	::rowWin := rw
  endif
  if undo
	::refresh()
  endif
return .t.
