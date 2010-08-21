static function te_prevWord(pos, line, newpos)
local ret:="", x, y, i, str, e:=.f., ch
	pos := iif(pos==NIL, ::pos, pos)
	line := iif(line==NIL, ::line, line)
	if valtype(line) == "N"
		::__check_line(line)
		str := ::edbuffer[line]
	else
		str := line
	endif
	x := pos
	for i:=pos to 1 step -1
		ch:= substr(str, i, 1)
		if !isalpha(ch) .and. !isdigit(ch) .and. ch != "_"
			pos := i
			e := .t.
			exit
		endif
	next
	if x == pos .and. pos == 1
		return ret
	endif
	x := 1
	y := 0
	for i:=pos to 1 step -1
		ch:= substr(str, i, 1)
		if e .and. (isalpha(ch) .or. isdigit(ch) .or. ch == "_")
			y := i
			e := .f.
		elseif !e .and. !(isalpha(ch) .or. isdigit(ch) .or. ch == "_")
			x := i+1
			exit
		endif
	next
	ret := substr(str, x, y-x+1)
	pos := x
	newpos := x
RETURN ret
