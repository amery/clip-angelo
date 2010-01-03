static function te_curWord(pos, line)
local ret:="", x, y, i, str, alpha, digit, ch
	pos := iif(pos==NIL, ::pos, pos)
	line := iif(line==NIL, ::line, line)
	if valtype(line) == "N"
		::__check_line(line)
		str := ::edbuffer[line]
	else
		str := line
	endif
	ch := substr(str, pos, 1)
	alpha := isalpha(ch)
	digit := isdigit(ch)
	if alpha .or. digit .or. !digit .and. (ch == '.' .or. ch == ',');
	   .and. isdigit(substr(str, pos+1, 1)) .or. ch =='_'
		x := pos
		y := pos
		for i:=pos-1 to 0 step -1
			ch := substr(str, i, 1)
			alpha := isalpha(ch)
			digit := isdigit(ch)
			if !alpha .and. !digit .and. ch != '_' .or. i==0
				x := i+1
				exit
			endif
		next
		for i:=pos+1 to len(str)+1
			ch := substr(str, i, 1)
			alpha := isalpha(ch)
			digit := isdigit(ch)
			if !alpha .and. !digit .and. ch != '_'
				y := i-1
				exit
			endif
		next
		ret := substr(str, x, y-x+1)
	endif
RETURN ret
