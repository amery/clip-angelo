static function db_close()
   if ::item:hasFocus
	::killFocus()
   endif
   asize(::inbuffer, 0)
   asize(::findR, 0)
   ::lines := 0
   ::n_Line := 1
   ::n_Pos := ::marginLeft
   ::Find := map()
   ::item := map()
   //setcursor(::__oldcursor)
   //setcolor(::__oldcolor)
   //restore screen from ::__oldscr
return len(::inbuffer)
