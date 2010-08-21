static function assign()
 local s,changed:=::changed
#ifdef DEBUG
	outlog(__FILE__,__LINE__,"assign",::name)
#endif
   if ::block==NIL
	return NIL
   endif
 if ::type $ GETS_TYPES
    s:=::unTransForm()
    eval(::block,s)
    //::setFocus()
    ::updateBuffer()
    ::changed:=changed
 endif
return NIL
