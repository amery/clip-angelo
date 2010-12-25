static function get_setcolor(color)
	if color != NIL
		::colorSpec := color
	endif
	::__colors:=__splitColors(::colorSpec)
	::colorSpec := ::__colors[5]+','+::__colors[2]
#ifdef DEBUG
	outlog(__FILE__,__LINE__,"setcolor",::name)
#endif
return NIL
