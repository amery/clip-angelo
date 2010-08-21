static function te_writeundo( cmd, val )
local st[11], prev

	prev:=max(1, ::__curundo)
	if ::__undobuffer[prev] != NIL .and. ::__undobuffer[prev][U_CMD] == cmd .and. val==NIL//(val==NIL .or. val==::__undobuffer[prev][U_VALUE])
		::__undobuffer[prev][U_CYCLE] ++
		return
	endif
	if ::__curundo < 1
		::__curundo := 1
	else
		::__curundo ++
	endif
	if ::__curundo > ::lenundo
		::__curundo := 1
		::__startundo ++
	endif
	::__startundo += iif(::__startundo>0, 1, 0)
	if ::__startundo > ::lenundo
		::__startundo := 0
	endif
	st[U_CMD] := cmd
	st[U_CYCLE] := 1
	st[U_POS] := ::pos
	st[U_LINE] := ::line
	st[U_LINES] := ::lines
	st[U_COL] := ::colWin
	st[U_ROW] := ::rowWin
	st[U_VALUE] := ""
	st[U_BLOCK] := {0, 0, 0, 0, 0}
	st[U_FIND] := aclone(::__findR)
	st[U_MKBLOCK] := ::mkblock

	do case
	case cmd == HASH_INS
		if ::__undobuffer[prev] != NIL .and. !valtype(::__undobuffer[prev][U_VALUE]) $ "AO" .and. ::__undobuffer[prev][U_VALUE] == val
			::__undobuffer[prev][U_CYCLE] ++
			::__curundo := prev
			return
		endif
		st[U_VALUE] := val
	case cmd == HASH_OVR
		/*
		if ::__undobuffer[prev] != NIL .and. !valtype(::__undobuffer[prev][U_VALUE]) $ "AO" .and. ::__undobuffer[prev][U_VALUE] == val
			::__undobuffer[prev][U_CYCLE] ++
			::__curundo := prev
			return
		endif
		*/
		st[U_VALUE] := val
	case cmd == HASH_INSAUTOWRAP .or. cmd == HASH_OVRAUTOWRAP
		st[U_VALUE] := aclone(val)
	case cmd == HASH_BS .or. cmd == HASH_DEL
		st[U_VALUE] := val
	case cmd == HASH_DELINE .or. cmd == HASH_INSLINE
		st[U_VALUE] := val
	case cmd == HASH_DELEND .or. cmd == HASH_DELHOME
		st[U_VALUE] := val
	case cmd == HASH_DELBL .or. cmd == HASH_MVBL
		st[U_VALUE] := aclone(val)
	case cmd == HASH_PASTCLB
		st[U_VALUE] := aclone(val)
	case cmd == HASH_FIND
		st[U_VALUE] := val
	case cmd == HASH_REPLACE
		st[U_VALUE] := val
	case cmd == HASH_CHECKLINE
		st[U_VALUE] := val
	case cmd == HASH_LOADBLOCK
		st[U_VALUE] := aclone(val)
	case cmd == HASH_FRLINE
		st[U_VALUE] := val
	case cmd == HASH_FRPART
		st[U_VALUE] := aclone(val)
	case cmd == HASH_DRAW
		st[U_VALUE] := aclone(val)
	case cmd == HASH_INSMACRO
		st[U_VALUE] := val
	case cmd == HASH_INSTEMPL
		st[U_VALUE] := aclone(val)
	case cmd == HASH_INSTAB .or. cmd == HASH_OVRTAB
		st[U_VALUE] := val
	endcase
	if ::strblock .or. ::rectblock
		st[U_BLOCK][1] := iif(::strblock, 1, 2)
		st[U_BLOCK][2] := ::koordblock[1]
		st[U_BLOCK][3] := ::koordblock[2]
		st[U_BLOCK][4] := ::koordblock[3]
		st[U_BLOCK][5] := ::koordblock[4]
	endif
	if len(::__undobuffer)<1 .or. len(::__undobuffer)<::__curundo
	   aadd(::__undobuffer, aclone(st) )
	else
	   ::__undobuffer[::__curundo] := aclone(st)
	endif
return
