static function te_insTempl(targ, undo)
local i, j, p, target, start, en, spl
	undo := iif(undo==NIL, .t., undo)

	if ::line > ::lines
		::__check_line(::line)
	endif
	p := ::pos
	while p>0 .and. empty(substr(::edbuffer[::line], p, 1))
		p--
	enddo
	start := p
	en := p
	for i:= start-1 to 1 step -1
		if substr(::edbuffer[::line], i, 1) == " "
			exit
		endif
	next
	start := i+1
	for i:= en+1 to len(::edbuffer[::line])
		if substr(::edbuffer[::line], i, 1) == " "
			exit
		endif
	next
	en := i+1
	target := upper(alltrim(substr(::edbuffer[::line], start, en-start+1)))
	if empty(target)
		return
	endif
	if target $ targ
		spl := split(targ[target], "/")
		if undo
			::writeundo(HASH_INSTEMPL, {::edbuffer[::line], len(spl)})
		endif
		::edbuffer[::line] := substr(::edbuffer[::line], 1, start-1) + spl[1] + substr(::edbuffer[::line], en)

		for j := 2 to len(spl)
			::lines++
			asize(::edbuffer, ::lines)
			ains(::edbuffer, ::line+j-1)
			::edbuffer[::line+j-1] := replicate(" ", start-1) + spl[j]
		next
		::gotoPos(len(::edbuffer[::line])+1, ,.f.)
		::refresh()
	endif
RETURN
