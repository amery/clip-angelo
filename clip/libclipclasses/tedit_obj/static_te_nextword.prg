static function te_nextWord(pos, line, newpos)
local ret:="", x, y, i, str, e:=.t., ch
	pos := iif(pos==NIL, ::pos, pos)
	line := iif(line==NIL, ::line, line)
	if valtype(line) == "N"
		::__check_line(line)
		str := ::edbuffer[line]
	else
		str := line
	endif
	for i:=pos to len(str)
		ch:= substr(str, i, 1)
		if !isalpha(ch) .and. !isdigit(ch) .and. ch != "_"
			pos := i
			exit
		endif
	next
	x := len(str)
	y := len(str)
	for i:=pos+1 to len(str)
		ch:= substr(str, i, 1)
		if e .and. (isalpha(ch) .or. isdigit(ch) .or. ch == "_")
			x := i
			y := len(str)+1
			e := .f.
		elseif !e .and. !(isalpha(ch) .or. isdigit(ch) .or. ch == "_")
			y := i
			exit
		endif
	next
	ret := substr(str, x, y-x)
	newpos := x
RETURN ret
