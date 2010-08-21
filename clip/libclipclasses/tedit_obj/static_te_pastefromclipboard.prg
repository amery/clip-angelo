static function te_pasteFromClipboard( Clipboard, it, undo )
local i, j
	if empty(Clipboard)
		return .f.
	endif
	it := iif(it==NIL, 1, it)
	undo := iif(undo==NIL, .t., undo)
	if undo
		::writeundo(HASH_PASTCLB, Clipboard[it])
	endif
	::line := ::__check_line(::line, .f.)
	/* �������� �������� ���� */
	if Clipboard[it][1]
		::lines += len(Clipboard[it][2])
		asize(::edbuffer, ::lines)
		::strblock := .t.
		::rectblock := .f.
		afill(::koordblock, NIL)
		::koordblock[1] := ::line
		::koordblock[2] := ::pos
		::koordblock[3] := ::line+len(Clipboard[it][2])-1
		::koordblock[4] := ::pos
		for i=1 to len(Clipboard[it][2])
			ains(::edbuffer, ::line+i-1)
			::edbuffer[::line+i-1] := Clipboard[it][2][i]
		next
	else /* �������� ������������� ���� */
		::strblock := .f.
		::rectblock := .t.
		afill(::koordblock, NIL)
		::koordblock[1] := ::line
		::koordblock[2] := ::pos
		::koordblock[3] := ::line+len(Clipboard[it][2]) - 1
		::koordblock[4] := ::pos + len(Clipboard[it][2][1]) - 1
		for i=1 to len(Clipboard[it][2])
			::__check_line(::line+i-1)
			::edbuffer[::line+i-1] := padr(::edbuffer[::line+i-1], ::pos-1)+;
					Clipboard[it][2][i]+;
					substr(::edbuffer[::line+i-1], ::pos)
		next
	endif
	::updated := .t.
	if undo
		::refresh()
	endif
RETURN .t.
