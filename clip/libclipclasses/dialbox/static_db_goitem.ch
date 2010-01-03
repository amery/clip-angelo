static function db_goItem(item, vref)
local i, j, found := .f., vt
	vref := iif(vref==NIL, .t., vref)
	vt := valtype(item)
	if item==NIL .or. (vt=="N" .and. (item < 1 .or. item > ::__nbr-1))
		return .f.
	endif
	::killFocus()

	for i:=1 to ::lines
		for j:=1 to len(::inbuffer[i])
			if (vt=="N" .and. ::inbuffer[i][j]:number == item) .or.;
			   (vt=="C" .and. ::inbuffer[i][j]:name == item)
				found := .t.
				::line := ::inbuffer[i][j]:nt
				::pos := ::inbuffer[i][j]:nl
				::colWin := ::pos
				::rowWin := ::line
				::item_num := j
				exit
			endif
		next
		if found
			exit
		endif
	next
	if found .and. vref
		::refresh()
	endif
return found
