static function varGet
	local s
#ifdef DEBUG
	outlog(__FILE__,__LINE__,"varget",::name)
#endif
	if ::block==NIL
		return NIL
	endif
#ifndef _______
	return eval(::block)
#else
	if ::type $ GETS_TYPES
		s:=eval(::block)
		return s
	endif
return ::_original
#endif
