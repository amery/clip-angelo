static function reset(obj)
#ifdef DEBUG
	outlog(__FILE__,__LINE__,"reset",obj:name)
#endif
  if obj:hasFocus
     //obj:changed:=.f.
     //obj:exitState:=GE_NOEXIT
     //obj:hasFocus:=.f.
     obj:pos:=1
     //obj:reader:=NIL
     obj:rejected:=.f.
     //obj:subscript:=NIL
     obj:typeOut:=.f.
     obj:clear:=.f.
     obj:minus:=.f.
     obj:__firstKey:=.f.
  endif
return NIL
