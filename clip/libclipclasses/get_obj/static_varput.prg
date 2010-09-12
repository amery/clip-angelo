static function varPut(value)
	local s
#ifdef DEBUG
	outlog(__FILE__,__LINE__,"varput",::name)
#endif
	if ::block==NIL
		return NIL
	endif
#ifndef _______
	return eval(::block,value)
#else
	if ::type $ GETS_TYPES
		s:=eval(::block,value)
		return s
	endif
return ::_original
#endif
