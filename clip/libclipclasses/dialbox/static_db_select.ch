static function db_select(direct)
local op, i, j, pos, eni, stj, enj, _step, nl:=0, nt:=0, nr:=0, nb:=0
/*
	if ::item:hasFocus()
		nl := ::inbuffer[::line][::item_num]:nl
		nr := ::inbuffer[::line][::item_num]:nr
		nt := ::inbuffer[::line][::item_num]:nt
		nb := ::inbuffer[::line][::item_num]:nb
	endif
	*/
//	::killFocus()
	direct := iif(direct==NIL, .t., .f.)
	if direct
		eni := len(::inbuffer)//::lines
		_step := 1
	else
		eni := 1
		_step := -1
	endif
	op := .f.
	pos := ::pos
	for i:= ::line to eni step _step
		stj := iif(direct, 1, len(::inbuffer[i]))
		enj := iif(direct, len(::inbuffer[i]), 1)
		for j:= stj to enj step _step
		/*
			if between(i, nt, nb) .and. between(::inbuffer[i][j]:nl, nl, nr)
				loop
			endif
		    */
			if ::inbuffer[i][j]:type > 0
				if (direct .and. ::inbuffer[i][j]:nl > pos) .or. (!direct .and. ::inbuffer[i][j]:nl < pos)
					pos := ::inbuffer[i][j]:nl
					op := .t.
					exit
				endif
			endif
		next
		if op
			exit
		endif
		pos := iif(direct, 1, iif(i-1>0, ::inbuffer[i-1][len(::inbuffer[i-1])]:nr, 0))
	next
	if op
		::killFocus()
		::colWin += pos - ::pos
		::rowWin += i - ::line
		::pos := pos
		::line := i
		::refresh()
	else
		alert("Object not found!")
	endif
return
