static function te_insert(str, undo)
   local expstr, p, pos, colWin, tailstr
   undo := iif(undo==NIL, .t., undo)
   str := iif(str==NIL, "", str)
   if ::line > ::lines
	::__check_line(::line)
   endif

   expstr := tabexpand(padr(::edbuffer[::line],::pos-1)+str, ::tabSize)
   tailstr := substr(::edbuffer[::line], ::pos)
   if str == chr(K_TAB)
       p := ::pos-1
       pos := ::pos+len(expstr)-p
       colWin := ::colWin+len(expstr)-p
   else
       pos := ::pos + len(str)
       colWin := ::colWin + len(str)
   endif

   if ::autoWrap .and. pos > ::marginRight
	return ::__autoWrap(undo, HASH_INSAUTOWRAP, expstr, tailstr, pos, colWin)
   endif

   if undo
	   if str == chr(K_TAB)
		::writeundo(HASH_INSTAB, ::edbuffer[::line])
	   else
		::writeundo(HASH_INS, str)
	   endif
   endif
   ::edbuffer[::line]=expstr+substr(::edbuffer[::line],::pos)

   ::pos := pos
   ::colWin := colWin

   ::updated:=.t.
   if undo
	::refresh()
   endif
return
