static function te_Home( undo )
local homepos:=0, lt
  undo := iif(undo==NIL, .t., undo)
  if ::line<=::lines
	lt := len(ltrim(::edbuffer[::line]))
	if lt == 0
		homepos := 1
	else
		homepos := len(::edbuffer[::line]) - lt + 1
	endif

  endif
  if undo
	::writeundo(HASH_HOME)
  endif
  if ::pos <= homepos
      ::pos := 1
      ::colWin := 1
  else
      ::colWin -= ::pos-homepos
      ::pos := homepos
  endif
  if undo
	::refresh()
  endif
return
