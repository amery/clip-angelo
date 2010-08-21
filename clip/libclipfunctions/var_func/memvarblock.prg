***************************************************************
function memvarblock(vname)
  local __cname
  if ismemvar(vname) //type("m->"+vname)!="U"
	 __cname:="{|_1| iif(_1==NIL,m->"+vname+",m->"+vname+":=_1) }"
	 __cname:=&__cname
	 return __cname
  endif
return NIL
