function fieldblock(fname)
  local __cname
  __cname:="{|_1| iif(_1==NIL, _field->"+fname+",_field->"+fname+":=_1) }"
  __cname:=&__cname
return __cname
