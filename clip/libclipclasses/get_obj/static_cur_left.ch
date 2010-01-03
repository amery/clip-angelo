static function cur_left()
   local p
#ifdef DEBUG
	outlog(__FILE__,__LINE__,"cur_left",::name)
#endif
   p:=ascan(::__posArr,::pos)
   p--
   if p<=0
      p:=1
      //::typeOut:=.t.
   endif
   p:=min(p,len(::__posArr))
   if p>0 .and. p<=len(::__posArr)
      ::pos:=::__posArr[p]
   endif
  ::__firstKey:=.t.
  ::_display()
return NIL
