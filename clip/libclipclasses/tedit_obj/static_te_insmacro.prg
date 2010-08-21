static function te_insMacro(targ, undo)
local i, j, p, target, start, en
	undo := iif (undo==NIL, .t., undo)
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
			if undo
				::writeUndo(HASH_INSMACRO, ::edbuffer[::line])
			endif
			::edbuffer[::line] := left(::edbuffer[::line], start-1);
			    + ToString(&(targ[target])) + substr(::edbuffer[::line], en)
			::gotoPos(start+len(ToString(&(targ[target])))+1, ,.f.)
			::refresh()
	endif
RETURN
