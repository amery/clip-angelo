static function te_Frmt(str, len, s1, s2, spl, firstPos)
local x, y, a1, a2, let, p, i
	x := len
	y := len
	let := "[���������aeiouy]"
	spl := iif(spl==NIL, .f., .t.)
	firstPos := iif(firstPos==NIL, 0, firstPos)
	if isalpha(substr(str, len, 1)) .or. isdigit(substr(str, len, 1)) .or. isdigit(::curword(len, str))
		/* ���� ����� ������� � ������ */
		if spl .and. !isdigit(::curword(len, str))
			p := .t.
		else
			p := .f.
		endif
		s1 := substr(str, 1, len-1)
		s2 := substr(str, len)
		x := len(s1)
		for i:=len-1 to 1 step -1
			if !isalpha(substr(s1, i, 1)) .and. !isdigit(substr(s1, i, 1)) .and. !isdigit(::curword(len, s1))
				x := i
				exit
			endif
		next
		y := 0
		for i:=1 to len(s2)
			if !isalpha(substr(s2, i, 1)) .and. !isdigit(substr(s2, i, 1)) .and. !isdigit(::curword(len, s2))
				y := i
				exit
			endif
		next
		a1 := {}
		a2 := {}
		while (y < 1 .or. !(search(let, substr(s2, 1, y), a2) .and. search("([a-z]+)([�-�]+)", substr(s2, 1, y), a2) )) .and. (len(s1)-x > 1)//(len(s1)-x >= 2)
			s2 := substr(s1, len(s1), 1) + s2
			s1 := substr(s1, 1, len(s1)-1)
			y ++
		enddo
		p := p .and. (len(s1)-x>1)
		p := p .and. search(let, substr(s1, x), a1)
		//p := p .and. search(let, substr(s2, 1, y), a2)

		if p
			s1 += "-"
		else
			s2 := substr(s1, x+1) + s2
			s1 := substr(s1, 1, x)
		endif
	else
		s1 := substr(str, 1, len)
		s2 := ltrim(substr(str, len+1))
	endif
	x := len - len(s1)
	s1 := rtrim(s1)
	/* �������� ������� ��� ������������ ����� ������ */
	if spl .and. x > 0 .and. !empty(s2)
		y := len(s1)
		for i:= 1 to x
			y := rat(" ", substr(s1, 1, y))
			if y > firstPos .and. y > 0
				y --
				s1 := substr(s1, 1, y)+" "+substr(s1, y+1)
			else
				y := len(s1)
			endif
		next
	endif
	s2 := alltrim(s2)
return
