static function te_find(Find, undo)
local str, scr
local i, p, f, found:=.f., rr, st, en, _step

	undo := iif(undo==NIL, .t., undo)
	if undo
		::writeundo(HASH_FIND, Find)
	endif
	//save screen to scr
	//@ maxrow(), 0 say padr([Find string...], maxcol()) color "0/7"
	//inkey()
	str := Find:fstring
	p := ::pos
	if Find:where == 2
		if !(::strBlock .or. ::rectBlock)
			//messagep("Block not found!")
			restore screen from scr
			return .f.
		endif
		if Find:direct == 2  // backward
			st := max(::koordBlock[1], ::koordBlock[3])
			en := min(::koordBlock[1], ::koordBlock[3])
			_step := -1
		else
			st := min(::koordBlock[1], ::koordBlock[3])
			en := max(::koordBlock[1], ::koordBlock[3])
			_step := 1
		endif
	else
		if Find:direct == 2  // backward
			st := min(::line, ::lines)
			en := 1
			_step := -1
		else
			st := min(::line, ::lines)
			en := ::lines
			_step := 1
		endif
	endif

	if !Find:case // ignore case
		str := upper(str)
	endif


	if Find:regexp
		for i = st to en step _step
			::__regSearch := {}
			f := search(str, iif(!Find:case, upper(::edbuffer[i]), ::edbuffer[i]), ::__regSearch, p)
			if f
				if Find:wordonly .and. (isalpha(substr(::edbuffer[i], ::__regSearch[1][1]-1, 1)) .or. isalpha(substr(::edbuffer[i], ::__regSearch[1][2], 1)))
					loop
				endif
				::__findR[1] := i               //line
				::__findR[2] := ::__regSearch[1][1]     //start pos
				::__findR[3] := ::__regSearch[1][2]-::__regSearch[1][1]//length
				::rowWin += i-(::line*_step)
				::line := i
				::gotoPos(::__regSearch[1][1], ,.f.)
				found := .t.
				exit
			endif
			p := 1
		next
	else
		for i = st to en step _step
			f := atl(str, substr(iif(!Find:case, upper(::edbuffer[i]), ::edbuffer[i]), p))
			if f > 0
				if Find:wordonly .and. (isalpha(substr(::edbuffer[i], p+f-2, 1)) .or. isalpha(substr(::edbuffer[i], p+f+len(str)-1, 1)))
					loop
				endif
				::__findR[1] := i       //line
				::__findR[2] := p+f-1   //start pos
				::__findR[3] := len(str)        //length
				::rowWin += i-(::line*_step)
				::line := i
				::gotoPos(p+f-1, ,.f.)
				found := .t.
				exit
			endif
			p := 1
		next
	endif
	/*
	if !found .and. !Find:allw .and. !(Find:rstring == NIL) .and. !Find:replace
		messagep([String;]+str+[;not found],,,"0/7")
	endif
	*/
	//restore screen from scr
	if undo
		::refresh()
	endif

RETURN found
