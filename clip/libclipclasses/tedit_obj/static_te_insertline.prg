static function te_insertLine(undo, autoIndent)
local str, mrL, ol
   undo := iif(undo==NIL, .t., undo)
   autoIndent := iif(autoIndent==NIL, .t., autoIndent)
   ol := ::lines
   ::__check_line(::line, undo)
   if undo
	::writeundo(HASH_INSLINE, ::edbuffer[::line])
   endif
   if ol < ::lines  // �.�. ������ ��� ��������� �������� check_line()
	if undo
		::refresh()
	endif
	return
   endif
   mrL := iif(::line >= 1, ;
		iif(autoindent, len(::edbuffer[::line])-len(ltrim(::edbuffer[::line]))+1, ::marginLeft),;
		1)
   str:=space(mrL-1)+substr(::edbuffer[::line],::pos)
   ::edbuffer[::line] := substr(::edbuffer[::line],1,::pos-1)
   aadd(::edBuffer,"")
   ::line++
   ::rowWin++
   ::pos:=::marginLeft
   ains(::edBuffer,::line)
   ::edBuffer[::line]:=str
   ::updated:=.t.
   ::lines:=len(::edbuffer)
   ::pos := mrL
   ::colWin := ::pos
   //::gotoPos(::pos)
   if undo
	::refresh()
   endif
return
