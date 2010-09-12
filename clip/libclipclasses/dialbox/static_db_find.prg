static function db_find()
local found := .f., i, j, str, p, f, st, en, _step, itemstr, pr
local elem, scr
	if empty(::Find:fstring)
		return found
	endif
	str := ::Find:fstring
	p := ::pos
	if ::Find:where == 2
		if !(::strBlock .or. ::rectBlock)
			//messagep("Block not found!")
			restore screen from scr
			return .f.
		endif
		if ::Find:direct == 2  // backward
			st := max(::koordBlock[1], ::koordBlock[3])
			en := min(::koordBlock[1], ::koordBlock[3])
			_step := -1
		else
			st := min(::koordBlock[1], ::koordBlock[3])
			en := max(::koordBlock[1], ::koordBlock[3])
			_step := 1
		endif
	else
		if ::Find:direct == 2  // backward
			st := min(::line, ::lines)
			en := 1
			_step := -1
		else
			st := min(::line, ::lines)
			en := ::lines
			_step := 1
		endif
	endif

	if !::Find:case // ignore case
		str := upper(str)
	endif


	if ::Find:regexp
		for i = st to en step _step
			for j=1 to len(::inbuffer[i])
				::regSearch := {}
				elem := ::inbuffer[i][j]
				itemstr := ::getString(elem, i)
				//itemstr := strtran(itemstr, "&", "")
				f := search(str, iif(!::Find:case, upper(itemstr), itemstr), ::regSearch, p)
				if f
					if ::Find:wordonly .and. (isalpha(substr(itemstr, ::regSearch[1][1]-1, 1)) .or. isalpha(substr(itemstr, ::regSearch[1][2], 1)))
						loop
					endif
					::findR[1] := i		//line
					::findR[2] := ::regSearch[1][1]	//start pos
					::findR[3] := ::regSearch[1][2]-::regSearch[1][1]//length
					do case
					case elem:type == _GETTEXT_
						elem:objit:edit:top()
						elem:objit:edit:find(::Find)
						::findR[1] := elem:nt + elem:objit:edit:rowWin - 1
						::findR[2] := elem:nl + elem:objit:edit:pos - 1
						::colWin += ::findR[2] - ::pos + 1
						::pos := elem:nl + elem:objit:edit:colWin
					otherwise
						::findR[2] += elem:nl -1
						::colWin += ::findR[2]-::pos + 1
						::pos := elem:nl
					endcase
					::rowWin += i-(::line*_step)
					::line := i
					found := .t.
					exit
				endif
				p := 1
			next
			if found
				exit
			endif
		next
	else
		for i = st to en step _step
			for j=1 to len(::inbuffer[i])
				elem := ::inbuffer[i][j]
				itemstr := ::getString(elem, i)
				f := atl(str, substr(iif(!::Find:case, upper(itemstr), itemstr), p))
				if f > 0
					if ::Find:wordonly .and. (isalpha(substr(itemstr, p+f-2, 1)) .or. isalpha(substr(itemstr, p+f+len(str)-1, 1)))
						loop
					endif
					::findR[1] := i	//line
					::findR[2] := p+f-1	//start pos
					::findR[3] := len(str)	//length
					do case
					case elem:type == _GETTEXT_
						elem:objit:edit:top()
						elem:objit:edit:find(::Find)
						::findR[1] := elem:nt + elem:objit:edit:rowWin - 1
						::findR[2] := elem:nl + elem:objit:edit:pos - 1
						::colWin += ::findR[2] - ::pos + 1
						::pos := elem:nl + elem:objit:edit:colWin
					otherwise
						::findR[2] += elem:nl -1
						::colWin += ::findR[2]-::pos + 1
						::pos := elem:nl
					endcase
					::rowWin += i-(::line*_step)
					::line := i
					found := .t.
					exit
				endif
				p := 1
			next
			if found
				exit
			endif
		next
	endif
	if found
		::refresh()
	endif
return found
