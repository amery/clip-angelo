static function killFocus()
#ifdef DEBUG
	outlog(__FILE__,__LINE__,"killfocus",::name)
#endif
   if ::block==NIL
	return NIL
   endif
  set("__readvar",::__oldReadVar)
  ::hasFocus:=.f.
  ::reset()
  ::_display()
  ::changed:=.f.
return NIL
