static function cur_setpos(nPos)
   local p
#ifdef DEBUG
	outlog(__FILE__,__LINE__,"cur_setpos",::name)
#endif
   if valtype(nPos) != "N"
	return NIL
   endif
   p:=ascan(::__posArr,nPos)
   if p<=0
	return NIL
   endif
   p:=min(p,len(::__posArr))
   if p>0 .and. p<=len(::__posArr)
      ::pos:=::__posArr[p]
   endif
  ::_display()
return NIL
