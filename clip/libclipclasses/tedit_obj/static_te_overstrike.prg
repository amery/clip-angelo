static function te_overStrike(str, undo)
   local expstr, p, k, pos, colWin, tailstr, srcchr
   undo := iif(undo==NIL, .t., undo)
   ::__check_line(::line)
   str := iif(str==NIL, "", str)
   if str == chr(K_TAB)
	if undo
		::writeundo(HASH_OVRTAB, "")
	endif
	p := int(::pos / ::tabSize)
	k := ::pos - p
	::colWin += (p+1) * ::tabSize - ::pos + 1
	::pos := (p+1) * ::tabSize + 1

	if undo
		::refresh()
	endif
	return
   endif
   expstr := tabexpand(padr(::edbuffer[::line],::pos-1)+str, ::tabSize)
   tailstr := substr(::edbuffer[::line], ::pos+1)
   srcchr := substr(::edbuffer[::line], ::pos, len(str))
   if undo
	::writeundo(HASH_OVR, srcchr)
   endif
   pos := ::pos + len(str)
   colWin := ::colWin + len(str)

   if ::autoWrap .and. pos > ::marginRight
	return ::__autoWrap(undo, HASH_OVRAUTOWRAP, expstr, tailstr, pos, colWin)
   endif


   ::edbuffer[::line]=expstr+tailstr

   ::pos := pos
   ::colWin := colWin

   ::updated:=.t.
   if undo
	::refresh()
   endif
return
