static function te___autoWrap(undo, undocmd, expstr, tailstr, pos, colWin)
   /* auto margin right */
local i, m, p, l, srcstr, parr
   undo := iif(undo==NIL, .t., undo)

   m:={}
   if int(::pos/::marginRight) == 1
	if undo
		aadd(m, 1)
		aadd(m, ::edbuffer[::line])
		::writeundo(undocmd, m)
	endif
	::edbuffer[::line]=expstr+tailstr

	::pos := pos
	::colWin := colWin
	p := 0
	do while (::pos > 1)
		::pos --
		::colWin --
		p ++
		if (::pos <= ::marginRight) .and. ;
		(substr(::edbuffer[::line], ::pos, 1) == " ")
			::pos ++
			::colWin ++
			::insertLine(.f., .t.)
			::pos += p - 1
			::colWin += p - 1
			exit
		endif
	enddo
   else
	srcstr := ::edbuffer[::line]

	l := 0
	parr := {}
	do while len (expstr) > ::marginRight
		p := ::marginRight
		do while p > 1
			if substr(expstr, p, 1) == " "
				exit
			endif
			p --
		enddo
		aadd(parr, substr(expstr, 1, p) )
		expstr := /*::space(::marginLeft -1)+*/substr(expstr, p+1)
		l ++
	enddo

	aadd(parr, expstr + tailstr )

	if undo
		aadd(m, l)
		aadd(m, srcstr)
		::writeundo(undocmd, m)
	endif
	::lines += l
	asize(::edbuffer, ::lines)
	::edbuffer[::line] := parr[1]
	for i=2 to len(parr)
		ains(::edbuffer, ::line + i - 1)
		::edbuffer[::line + i - 1] := parr[i]
	next
	::line += l
	::rowWin += l
	::pos := ::colWin := len(expstr) + 1

   endif
   ::updated:=.t.
   if undo
	::refresh()
   endif
return
