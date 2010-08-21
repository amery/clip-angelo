static function te_replace(Find, undo)
local found:=.t., cstr:="", nkey, i, ret:=-1

	undo := iif(undo==NIL, .t., undo)
	cstr := iif(Find:rstring==NIL, cstr, Find:rstring)
	found := ::find(Find, .f.)
	if found
		ret := 1
		::refresh()
		nkey := iif (undo, inkey(0), K_ENTER)
		do case
			case nkey == K_ESC
				ret := -1
				return ret
			case nkey != K_ENTER
				ret := 0
				return ret
		endcase
		if Find:regexp
			if len(::__regSearch) == 1
				cstr := strtran(cstr, "\1", substr(::edbuffer[::__findR[1]], ::__regSearch[1][1], ::__regSearch[1][2]-::__regSearch[1][1]))
			else
				for i=1 to len(::__regSearch)-1
					cstr := strtran(cstr, "\"+alltrim(tostring(i)), substr(::edbuffer[::__findR[1]], ::__regSearch[i+1][1], ::__regSearch[i+1][2]-::__regSearch[i+1][1]))
				next
			endif
		endif
		if undo
			::writeundo(HASH_REPLACE, Find)// {substr(::edbuffer[::__findR[1]], ::__findR[2], ::__findR[3]), cstr})
		endif
		::edbuffer[::__findR[1]] := substr(::edbuffer[::__findR[1]], 1, ::__findR[2]-1)+;
					  cstr + substr(::edbuffer[::__findR[1]], ::__findR[2]+::__findR[3])
		::cancelBlock(.f.)
		if undo
			::refresh()
		endif
		::updated := .t.
		::gotoPos(::pos + len(cstr), ,.f. )
	endif
RETURN ret
