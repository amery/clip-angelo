static function te_matchStruct(curword, forward, undo)
local found:=.f., f_found, prevword, nextword, ind:=0, arr:={}, i, cnt:=0, cntoth:=0
local clip_synt_beg, clip_synt_end, clip_synt_loop, word
local si, ei, ss, is_beg:=.f., is_end:=.f., is_loop:=.f., is, o_com:=0
local pat_beg:="", pat_end:="", pat_loop:="", str, ch, r
memvar st
private st
	forward := iif(forward==NIL, .t., forward)
	undo := iif(undo==NIL, .t., .f.)
	if undo
		::writeundo(HASH_FIND, -1)
	endif
	clip_synt_beg := {"(DO WHILE)|(WHILE)",;
			  "(BEGIN SEQUENCE)",;
			  "(FOR)",;
			  "(IF)",;
			  "(DO CASE)"}
	clip_synt_end := {"(END)|(ENDDO)", ;
			  "(END SEQUENCE)",;
			  "(NEXT)",;
			  "(END)|(ENDIF)",;
			  "(END)|(ENDCASE)"}
	clip_synt_loop := {"(EXIT)|(LOOP)",;
			   "(BREAK)|(RECOVER)",;
			   "(EXIT)|(LOOP)",;
			   "(ELSEIF)|(ELSE)",;
			   "(CASE)|(OTHERWISE)|(EXIT)"}
	::__check_line(::line)
	st := ::pos
	curword := upper(curword)
	prevword := upper(::prevWord())
	nextword := upper(::nextWord())
	if empty(curword) .and. ! empty(prevword)
		curword := prevword
	endif
	do case
		case curword == "DO"
			curword += " "+nextword
			st += len(nextword)
		case curword == "WHILE"
			curword := "DO "+curword
		case curword == "CASE" .and. prevword == "DO"
			curword := "DO "+curword
		case curword == "BEGIN" .and. nextword == "SEQUENCE"
			curword += " "+nextword
			st += len(nextword)
		case curword == "SEQUENCE" .and. prevword == "BEGIN"
			curword := prevword+" "+curword
	endcase
	for i=1 to len(clip_synt_beg)
		is := search(clip_synt_beg[i], curword) .and. search(curword, clip_synt_beg[i])
		pat_beg += clip_synt_beg[i] + "|"
		if is
			ind := i
			is_beg := .t.
		endif
	next
	for i=1 to len(clip_synt_end)
		is := search(clip_synt_end[i], curword) .and. search(curword, clip_synt_end[i])
		pat_end += clip_synt_end[i] + "|"
		if is
			ind := i
			is_end := .t.
		endif
	next
	for i=1 to len(clip_synt_loop)
		is := search(clip_synt_loop[i], curword) .and. search(curword, clip_synt_loop[i])
		pat_loop += clip_synt_loop[i] + "|"
		if is .and. !(is_beg .or. is_end)
			ind := i
			is_loop := .t.
		endif
	next
	pat_beg := left(pat_beg, len(pat_beg)-1)
	pat_end := left(pat_end, len(pat_end)-1)
	pat_loop := left(pat_loop, len(pat_loop)-1)

	if !(is_beg .or. is_end .or. is_loop)
		return found
	endif

	if is_beg
		forward := .t.
		cnt ++
	elseif is_end
		forward := .f.
		cnt --
		if curword == "END"
			ind := 1
			clip_synt_beg[ind] := "(DO WHILE)|(WHILE)|(DO CASE)|(IF)"
			clip_synt_end[ind] := "(END)|(ENDDO)|(ENDIF)|(ENDCASE)"
			clip_synt_loop[ind] := "(EXIT)|(LOOP)|(ELSE)|(ELSEIF)"
		endif
	else
		cnt += iif(forward, 1, -1)
		do case
		case curword == "EXIT"
			ind := 1
			clip_synt_beg[ind] := "(DO WHILE)|(WHILE)|(FOR)|(DO CASE)"
			clip_synt_end[ind] := "(END)|(ENDDO)|(NEXT)|(ENDCASE)"
			clip_synt_loop[ind] := "(EXIT)|(LOOP)|(CASE)|(OTHERWISE)"
		case curword == "LOOP"
			ind := 1
			clip_synt_beg[ind] := "(DO WHILE)|(FOR)"
			clip_synt_end[ind] := "(END)|(ENDDO)|(NEXT)"
			clip_synt_loop[ind] := "(EXIT)|(LOOP)"
		endcase
	endif
	if forward
		si := ::line
		ei := ::lines
		ss := 1
	else
		si := ::line
		ei := 1
		ss := -1
	endif
	f_found := .f.
	for i:=si to ei step ss
		if !forward
			r := -(len(::edbuffer[i]) - st + 1)
		else
			r := len(::edbuffer[i])
		endif
		asize(arr, 0)

		str := upper(::edbuffer[i])

		if search("//", str, arr, st, r)
			str := substr(str, 1, arr[1][1])
			asize(arr, 0)
		endif
		do while search('".+"', str, arr)
			str := substr(str, 1, arr[1][1])+replicate(".", arr[1][2]-arr[1][1])+substr(str, arr[1][2])
			asize(arr, 0)
		enddo
		do while search("'.+'", str, arr)
			str := substr(str, 1, arr[1][1])+replicate(".", arr[1][2]-arr[1][1])+substr(str, arr[1][2])
			asize(arr, 0)
		enddo
		if search("/[*]", str, arr, st, r)
			o_com ++
			if search("[*]/", str, arr, arr[1][1]+1, r+arr[1][1])
				o_com --
				str := substr(str, 1, arr[1][1]-1)+padl(substr(str, arr[2][2]), arr[2][2]-arr[1][1])
			else
				str := substr(str, 1, arr[1][1]-1)
			endif
		elseif search("[*]/", str, arr, st, r)
			o_com --
			if search("/[*]", str, arr, arr[1][1], r+arr[1][1])
				o_com ++
				str := substr(str, 1, arr[2][1])+padl(substr(str, arr[1][1]), arr[1][1]-arr[2][1]+1)
			else
				str := padl(substr(str, arr[1][2]), len(str))
			endif
		endif
		asize(arr, 0)

		if substr(alltrim(str), 1, 1) == "*"
			st := 0
			loop
		endif

		if len(alltrim(str)) == 0// .or. o_com > 0
			st := 0
			loop
		endif
		if !forward .and. st == 0
			st := len(str)+1
		endif
		word := upper(iif(forward, ::nextWord(st, str, @st), ::prevWord(st, str, @st)))
		while !empty(word)
			found := .f.
			do case
				case word == "DO"
					nextword := upper(::nextWord(st,str))
					word += " "+nextword
					st += iif(forward, len(nextword)-1, -1)
				case word == "WHILE"
					word := "DO "+word
					st += iif(upper(::prevWord(st, str)) == "DO", -3, 0)
				case word == "CASE" .and. upper(::prevWord(st, str)) == "DO"
					word := "DO "+word
					st -= 3
				case word == "BEGIN" .and. upper(::nextWord(st, str)) == "SEQUENCE"
					word += " SEQUENCE"
					st += 8
				case word == "SEQUENCE" .and. upper(::prevWord(st, str)) == "BEGIN"
					word := "BEGIN "+word
					st -= 6
			endcase
			if search(clip_synt_beg[ind], word) .and. search(word, clip_synt_beg[ind])
				cnt ++
				found := .t.
			elseif search(clip_synt_end[ind], word) .and. search(word, clip_synt_end[ind])
				cnt --
				found := .t.
			elseif search(clip_synt_loop[ind], word) .and. search(word, clip_synt_loop[ind])
				if cntoth == 0 .and. between(cnt, -1, 1)
					cnt += iif(forward, -1, 1)
				endif
				found := .t.
			elseif search(pat_beg, word) .and. search(word, pat_beg)
				cntoth ++
			elseif search(pat_end, word) .and. search(word, pat_end)
				cntoth --
			elseif search(pat_loop, word) .and. search(word, pat_loop)
				found := .t.
			endif

			if found .and. cntoth == 0 .and. cnt == 0 .and. o_com == 0
				f_found := .t.
				exit
			endif
			st += iif(forward, len(word), -1)
			word := upper(iif(forward, ::nextWord(st, str, @st), ::prevWord(st, str, @st)))
		enddo
		if f_found
			::__findR[1]:= i
			::__findR[2]:= st
			::__findR[3]:= len(word)
			found := .t.
			::rowWin += i - ::line
			::line := i
			::colWin += st - ::pos
			::pos := st
			exit
		endif
		st := 0
	next


	if undo
		::refresh()
	endif
return found
