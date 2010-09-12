static function  forceStable
#ifdef DEBUG_CALL
	outlog(__FILE__,__LINE__,"forceStable")
#endif
	dispbegin()
	   do while !::stabilize()
	   enddo
	dispend()
return NIL
