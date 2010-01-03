static function db_destroy()
   if !empty(::item)
	::close()
   endif
   setcursor(::__oldcursor)
   setcolor(::__oldcolor)
   //restore screen from ::__oldscr
return
