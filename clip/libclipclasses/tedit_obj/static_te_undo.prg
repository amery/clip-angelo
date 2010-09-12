static function te_undo()
local i, j, cmd, len, line, p, edb
	if ::__curundo == ::__startundo
		return
	endif
	cmd := ::__undobuffer[::__curundo][U_CMD]
	for i=1 to ::__undobuffer[::__curundo][U_CYCLE]
		do case
		case cmd == HASH_UP
			::down(.f.)
		case cmd == HASH_DOWN
			::up(.f.)
		case cmd == HASH_CLEFT
			::right(.f.)
		case cmd == HASH_CRIGHT
			::left(.f.)
		case cmd == HASH_PGUP
			//::pageDown(.f.)
			::gotoLine(::__undobuffer[::__curundo][U_LINE], ::__undobuffer[::__curundo][U_ROW], .f.)
		case cmd == HASH_PGDOWN
			//::pageUp(.f.)
			::gotoLine(::__undobuffer[::__curundo][U_LINE], ::__undobuffer[::__curundo][U_ROW], .f.)
		case cmd == HASH_HOME
			::gotoPos(::__undobuffer[::__curundo][U_POS], ::__undobuffer[::__curundo][U_COL], .f.)
		case cmd == HASH_END
			::gotoPos(::__undobuffer[::__curundo][U_POS], ::__undobuffer[::__curundo][U_COL], .f.)
		case cmd == HASH_BOTOP
			::gotoLine(::__undobuffer[::__curundo][U_LINE], ::__undobuffer[::__curundo][U_ROW], .f.)
			::gotoPos(::__undobuffer[::__curundo][U_POS], ::__undobuffer[::__curundo][U_COL], .f.)
		case cmd == HASH_GOPOS
			::gotoPos(::__undobuffer[::__curundo][U_POS], ::__undobuffer[::__curundo][U_COL], .f.)
		case cmd == HASH_GOLINE
			::gotoLine(::__undobuffer[::__curundo][U_LINE], ::__undobuffer[::__curundo][U_ROW], .f.)
		case cmd == HASH_INS
			for j=1 to len(::__undobuffer[::__curundo][U_VALUE])
				::backspace(.f.)
			next
			//::gotoPos(::__undobuffer[::__curundo][U_POS], .f.)
		case cmd == HASH_OVR
			::gotoLine(::__undobuffer[::__curundo][U_LINE], ::__undobuffer[::__curundo][U_ROW], .f.)
			::gotoPos(::__undobuffer[::__curundo][U_POS], ::__undobuffer[::__curundo][U_COL], .f.)
			::overStrike(::__undobuffer[::__curundo][U_VALUE],.f.)
			::gotoPos(::__undobuffer[::__curundo][U_POS], ::__undobuffer[::__curundo][U_COL], .f.)
		case cmd == HASH_INSTAB
			::edbuffer[::__undobuffer[::__curundo][U_LINE]] := ::__undobuffer[::__curundo][U_VALUE]
			::gotoLine(::__undobuffer[::__curundo][U_LINE], ::__undobuffer[::__curundo][U_ROW], .f.)
			::gotoPos(::__undobuffer[::__curundo][U_POS], ::__undobuffer[::__curundo][U_COL], .f.)
			//::overStrike(::__undobuffer[::__curundo][U_VALUE],.f.)
			::gotoPos(::__undobuffer[::__curundo][U_POS], ::__undobuffer[::__curundo][U_COL], .f.)
		case cmd == HASH_OVRTAB
			::gotoPos(::__undobuffer[::__curundo][U_POS], ::__undobuffer[::__curundo][U_COL], .f.)
		case cmd == HASH_BS
			if ::__undobuffer[::__curundo][U_VALUE] == chr(K_ENTER)
				::insertLine(.f.)
			else
				::insert(::__undobuffer[::__curundo][U_VALUE], .f.)
			endif
		case cmd == HASH_DEL
			if ::__undobuffer[::__curundo][U_VALUE] == chr(K_ENTER)
				::insertLine(.f.)
				::gotoLine(::__undobuffer[::__curundo][U_LINE], ::__undobuffer[::__curundo][U_ROW], .f.)
				::gotoPos(::__undobuffer[::__curundo][U_POS], ::__undobuffer[::__curundo][U_COL], .f.)
			else
				::insert(::__undobuffer[::__curundo][U_VALUE], .f.)
				::gotoPos(::__undobuffer[::__curundo][U_POS], ::__undobuffer[::__curundo][U_COL], .f.)
			endif
		case cmd == HASH_DELEND
			::gotoLine(::__undobuffer[::__curundo][U_LINE], ::__undobuffer[::__curundo][U_ROW], .f.)
			::gotoPos(::__undobuffer[::__curundo][U_POS], ::__undobuffer[::__curundo][U_COL], .f.)
			::edbuffer[::line] += ::__undobuffer[::__curundo][U_VALUE]
		case cmd == HASH_DELHOME
			::line := ::__undobuffer[::__curundo][U_LINE]
			::pos := ::__undobuffer[::__curundo][U_POS]
			::edbuffer[::line] := ::__undobuffer[::__curundo][U_VALUE]+::edbuffer[::line]
		case cmd == HASH_DELINE
			::insertLine(.f.)
			::gotoLine(::__undobuffer[::__curundo][U_LINE], ::__undobuffer[::__curundo][U_ROW], .f.)
			::gotoPos(::__undobuffer[::__curundo][U_POS], ::__undobuffer[::__curundo][U_COL], .f.)
			::edbuffer[::__undobuffer[::__curundo][U_LINE]] := ::__undobuffer[::__curundo][U_VALUE]
		case cmd == HASH_INSLINE
			if len(::edbuffer) > 0
				::edbuffer[::__undobuffer[::__curundo][U_LINE]] := ::__undobuffer[::__curundo][U_VALUE]
				::deleteLine(.f.)
				::gotoLine(::__undobuffer[::__curundo][U_LINE], ::__undobuffer[::__curundo][U_ROW], .f.)
				::gotoPos(::__undobuffer[::__curundo][U_POS], ::__undobuffer[::__curundo][U_COL], .f.)
			endif
		case cmd == HASH_NEWLINE
			::gotoLine(::__undobuffer[::__curundo][U_LINE], ::__undobuffer[::__curundo][U_ROW], .f.)
			::gotoPos(::__undobuffer[::__curundo][U_POS], ::__undobuffer[::__curundo][U_COL], .f.)
		case cmd == HASH_BEGBLOCK
			::cancelBlock(.f.)
		case cmd == HASH_DELBL .or. cmd == HASH_MVBL .or. cmd == HASH_CPBL
			if cmd != HASH_DELBL
				::deleteBlock(.f.)
			endif
			line := min(::__undobuffer[::__curundo][U_BLOCK][2], ::__undobuffer[::__curundo][U_BLOCK][4])
			edb := ::edbuffer
			len := ::lines
			if ::__undobuffer[::__curundo][U_BLOCK][1] == 1
				aeval(::__undobuffer[::__curundo][U_VALUE], ;
					{|item| len:=len+1, asize(edb, len), ;
					 ains(edb, line), edb[line]:=item, ;
					 line:=line+1})
			else
				j := ::__undobuffer[::__curundo][U_BLOCK][3]
				aeval(::__undobuffer[::__curundo][U_VALUE], ;
					{|item| ;
					 edb[line]:=substr(edb[line], 1, j-1)+ item + substr(edb[line], j), ;
					 line:=line+1})
			endif
			::edbuffer := edb
			::lines := len(::edbuffer)
			::gotoLine(::__undobuffer[::__curundo][U_LINE], ::__undobuffer[::__curundo][U_ROW], .f.)
			::gotoPos(::__undobuffer[::__curundo][U_POS], ::__undobuffer[::__curundo][U_COL], .f.)
		case cmd == HASH_PASTCLB
			::deleteBlock(.f.)
			::lines := ::__undobuffer[::__curundo][U_LINES]
			asize(::edbuffer, ::__undobuffer[::__curundo][U_LINES])
		case cmd == HASH_FIND
			::gotoLine(::__undobuffer[::__curundo][U_LINE], ::__undobuffer[::__curundo][U_ROW], .f.)
			::gotoPos(::__undobuffer[::__curundo][U_POS], ::__undobuffer[::__curundo][U_COL], .f.)
		case cmd == HASH_REPLACE
			::gotoLine(::__undobuffer[::__curundo][U_LINE], ::__undobuffer[::__curundo][U_ROW], .f.)
			::gotoPos(::__undobuffer[::__curundo][U_POS], ::__undobuffer[::__curundo][U_COL], .f.)
			p := ::__undobuffer[::__curundo][U_VALUE]:fstring
			::__undobuffer[::__curundo][U_VALUE]:fstring := ::__undobuffer[::__curundo][U_VALUE]:rstring
			::__undobuffer[::__curundo][U_VALUE]:rstring := p
			::replace(::__undobuffer[::__curundo][U_VALUE], .f.)
		case cmd == HASH_CHECKLINE
			::gotoLine(::__undobuffer[::__curundo][U_LINE], ::__undobuffer[::__curundo][U_ROW], .f.)
			::gotoPos(::__undobuffer[::__curundo][U_POS], ::__undobuffer[::__curundo][U_COL], .f.)
			asize(::edbuffer, ::__undobuffer[::__curundo][U_VALUE])
			::lines := ::__undobuffer[::__curundo][U_VALUE]
		case cmd == HASH_LOADBLOCK
			::gotoLine(::__undobuffer[::__curundo][U_LINE], ::__undobuffer[::__curundo][U_ROW], .f.)
			::gotoPos(::__undobuffer[::__curundo][U_POS], ::__undobuffer[::__curundo][U_COL], .f.)
			edb := ::edbuffer
			j:=::__undobuffer[::__curundo][U_VALUE][1]
			aeval(::edbuffer, {|item| edb[j]:=item, j:=j+1}, ;
			      ::__undobuffer[::__curundo][U_VALUE][2]+1 )
			::edbuffer := edb
			::lines := len(::edbuffer)-::__undobuffer[::__curundo][U_VALUE][2]+::__undobuffer[::__curundo][U_VALUE][1]-1
			asize(::edbuffer, ::lines)
			::strblock := .f.
			::rectblock := .f.
		case cmd == HASH_FRLINE
			::lines -= 1
			::edbuffer[::__undobuffer[::__curundo][U_LINE]] := ::__undobuffer[::__curundo][U_VALUE]
			adel(::edbuffer, ::__undobuffer[::__curundo][U_LINE]+1)
			::gotoLine(::__undobuffer[::__curundo][U_LINE], ::__undobuffer[::__curundo][U_ROW], .f.)
			::gotoPos(::__undobuffer[::__curundo][U_POS], ::__undobuffer[::__curundo][U_COL], .f.)
			asize(::edbuffer, ::lines)
		case cmd == HASH_FRPART
			p := 0
			for j:= ::line to ::__undobuffer[::__curundo][U_LINE] step -1
				adel(::edbuffer, j)
				p++
			next
			::lines -= p
			::lines += len(::__undobuffer[::__curundo][U_VALUE])
			asize(::edbuffer, ::lines)
			for j:=1 to len(::__undobuffer[::__curundo][U_VALUE])
				ains(::edbuffer, ::__undobuffer[::__curundo][U_LINE]+j-1)
				::edbuffer[::__undobuffer[::__curundo][U_LINE]+j-1] := ::__undobuffer[::__curundo][U_VALUE][j]
			next
			::gotoLine(::__undobuffer[::__curundo][U_LINE], ::__undobuffer[::__curundo][U_ROW], .f.)
			::gotoPos(::__undobuffer[::__curundo][U_POS], ::__undobuffer[::__curundo][U_COL], .f.)
		case cmd == HASH_DRAW
			::gotoLine(::__undobuffer[::__curundo][U_LINE], ::__undobuffer[::__curundo][U_ROW], .f.)
			::gotoPos(::__undobuffer[::__curundo][U_POS], ::__undobuffer[::__curundo][U_COL], .f.)
			::edbuffer[::line] := stuff(::edbuffer[::line], ::pos+1, 1, ::__undobuffer[::__curundo][U_VALUE][1])
			::edbuffer[::line] := stuff(::edbuffer[::line], ::pos-1, 1, ::__undobuffer[::__curundo][U_VALUE][2])
			::edbuffer[::line-1] := stuff(::edbuffer[::line-1], ::pos, 1, ::__undobuffer[::__curundo][U_VALUE][3])
			::edbuffer[::line+1] := stuff(::edbuffer[::line+1], ::pos, 1, ::__undobuffer[::__curundo][U_VALUE][4])
			::edbuffer[::line] := stuff(::edbuffer[::line], ::pos, 1, ::__undobuffer[::__curundo][U_VALUE][5])
			::rowWin := ::__undobuffer[::__curundo][U_ROW]
			::colWin := ::__undobuffer[::__curundo][U_COL]
		case cmd == HASH_INSMACRO
			::gotoLine(::__undobuffer[::__curundo][U_LINE], ::__undobuffer[::__curundo][U_ROW], .f.)
			::edbuffer[::line] := ::__undobuffer[::__curundo][U_VALUE]
			::gotoPos(::__undobuffer[::__curundo][U_POS], ::__undobuffer[::__curundo][U_COL], .f.)
		case cmd == HASH_INSTEMPL
			for j=2 to ::__undobuffer[::__curundo][U_VALUE][2]
				::gotoLine(::__undobuffer[::__curundo][U_LINE]+1,, .f.)
				::deleteLine(.f.)
			next
			::gotoLine(::__undobuffer[::__curundo][U_LINE], ::__undobuffer[::__curundo][U_ROW], .f.)
			::edbuffer[::line] := ::__undobuffer[::__curundo][U_VALUE][1]
			::gotoPos(::__undobuffer[::__curundo][U_POS], ::__undobuffer[::__curundo][U_COL], .f.)
		case cmd == HASH_INSAUTOWRAP .or. cmd == HASH_OVRAUTOWRAP
			if len(::edbuffer) > 0
				::gotoLine(::__undobuffer[::__curundo][U_LINE],, .f.)
				for j=1 to ::__undobuffer[::__curundo][U_VALUE][1]
					::deleteLine(.f.)
				next
				::edbuffer[::__undobuffer[::__curundo][U_LINE]] := ::__undobuffer[::__curundo][U_VALUE][2]
				::gotoPos(::__undobuffer[::__curundo][U_POS], ::__undobuffer[::__curundo][U_COL], .f.)
			endif

		case cmd == HASH_MGOTO
			::mGoto(::__undobuffer[::__curundo][U_ROW], ::__undobuffer[::__curundo][U_COL],.f.)
		endcase
		//::rowWin := ::__undobuffer[::__curundo][U_ROW]
		//::colWin := ::__undobuffer[::__curundo][U_COL]
		if ::__undobuffer[::__curundo][U_BLOCK][1] > 0   //block
			if ::__undobuffer[::__curundo][U_BLOCK][1] == 1
				::strblock := .t.
			else
				::rectblock := .t.
			endif
			::koordblock[1] := ::__undobuffer[::__curundo][U_BLOCK][2]
			::koordblock[2] := ::__undobuffer[::__curundo][U_BLOCK][3]
			::koordblock[3] := ::__undobuffer[::__curundo][U_BLOCK][4]
			::koordblock[4] := ::__undobuffer[::__curundo][U_BLOCK][5]
		endif
		::mkblock := ::__undobuffer[::__curundo][U_MKBLOCK]
		::__findR[1] := ::__undobuffer[::__curundo][U_FIND][1]
		::__findR[2] := ::__undobuffer[::__curundo][U_FIND][2]
		::__findR[3] := ::__undobuffer[::__curundo][U_FIND][3]
	next
	::__curundo --
	if ::__curundo == 1 .and. ::__startundo > 0
		::__curundo := ::lenundo
	endif
	::refresh()
return
