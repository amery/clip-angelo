static function te_end( undo )
   undo := iif(undo==NIL, .t., undo)
   if undo
	::writeundo(HASH_END)
   endif
   //::gotoPos(len(::edBuffer[::line])+1, .f.)
   if ::line<=::lines
	::pos := len(rtrim(::edBuffer[::line]))+1
	::colWin := len(rtrim(::edBuffer[::line]))+1
   else
	::pos := 1
	::colWin := 1
   endif
   if undo
	::refresh()
   endif
return
