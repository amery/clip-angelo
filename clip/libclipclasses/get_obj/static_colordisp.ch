static function colorDisp (color)
#ifdef DEBUG
	outlog(__FILE__,__LINE__,"colordisp",::name)
#endif
   if ::block==NIL
	return NIL
   endif
  if color!=NIL
     ::colorSpec:=color
  endif
  ::setcolor()
  ::_display()
return ::classname
