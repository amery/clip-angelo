static function cur_home()
#ifdef DEBUG
	outlog(__FILE__,__LINE__,"cur_home",::name)
#endif
   if len(::__posArr) >0
	::pos:=::__posArr[1]
   endif
  ::__firstKey:=.t.
  ::_display()
return NIL
