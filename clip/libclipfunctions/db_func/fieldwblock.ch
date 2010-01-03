function fieldwblock(fname,anum)
  local __cname, __al:=alias(anum)
  __cname:="{|_1| iif(_1==NIL,"+__al+"->"+fname+","+__al+"->"+fname+":=_1) }"
  __cname:=&__cname
return __cname
