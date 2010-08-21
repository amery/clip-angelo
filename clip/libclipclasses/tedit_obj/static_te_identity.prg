static function te_Identity(symb, forward, undo)
local found := .f., dbl:=.f.
local i, j:=0, x, str1, str2, cnt:=0, ipos, iline, char, invchar, c
	symb := ""
	if ::line > ::lines
		return .f.
	endif
	forward := iif(forward==NIL, .t., forward)
	undo := iif(undo==NIL, .t., undo)
	if undo
		::writeundo(HASH_FIND, -1)
	endif
	i := ::pos
	str1 := "({[<"
	str2 := ")}]>"

	char := substr(::edbuffer[::line], ::pos, 1)

	j := at(char, str1)
	if j > 0
		invchar := substr(str2, j, 1)
		dbl := .t.
		cnt := 1
	else
		j := at(char, str2)
		if j > 0
			invchar := substr(str1, j, 1)
			dbl := .t.
			forward := !forward
			cnt := 1
		else
			invchar := char
		endif
	endif
	symb := invchar
	if dbl
		if forward
			i ++
			for j:=::line to ::lines
				for x:=i to len(::edbuffer[j])
					c := substr(::edbuffer[j], x, 1)
					if c == char
						cnt ++
						loop
					endif
					if c == invchar
						cnt --
					endif
					if cnt == 0
						ipos := x
						iline := j
						found := .t.
						exit
					endif
				next
				if found
					exit
				endif
				i := 1
			next
		else
			i--
			for j:=::line to 1 step -1
				for x:=i to 1 step -1
					c := substr(::edbuffer[j], x, 1)
					if c == char
						cnt ++
						loop
					endif
					if c == invchar
						cnt --
					endif
					if cnt == 0
						ipos := x
						iline := j
						found := .t.
						exit
					endif
				next
				if found
					exit
				endif
				if j-1 > 0
					i := len(::edbuffer[j-1])
				endif
			next
		endif
	else
		if forward
			i ++
			for j:=::line to ::lines
				for x:=i to len(::edbuffer[j])
					if substr(::edbuffer[j], x, 1) == char
						ipos := x
						iline := j
						found := .t.
						exit
					endif
				next
				if found
					exit
				endif
				i := 1
			next
		else
			i --
			for j:=::line to 1 step -1
				for x:=i to 1 step -1
					if substr(::edbuffer[j], x, 1) == char
						ipos := x
						iline := j
						found := .t.
						exit
					endif
				next
				if found
					exit
				endif
				if j-1 > 0
					i := len(::edbuffer[j-1])
				endif
			next
		endif
	endif
	if found
		::__findR[1] := iline   //line
		::__findR[2] := ipos    //start pos
		::__findR[3] := 1               //length
		::gotoPos(ipos, ,.f.)
		::gotoLine(iline, ,.f.)
		if undo
			::refresh()
		endif
	endif
return found
