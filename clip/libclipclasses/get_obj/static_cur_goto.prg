static function cur_goto(pos)
   local p:=pos
#ifdef DEBUG
	outlog(__FILE__,__LINE__,"cur_goto",::name)
#endif
//   p:=ascan(::__posArr,::pos)
//   p++
   if p>len(::__posArr)
      p:=len(::__posArr)
      ::typeOut:=.t.
   endif
   p:=max(1,p)
   if p>0 .and. p<=len(::__posArr)
      ::pos:=::__posArr[p]
   endif
  ::__firstKey:=.t.
   ::_display()
return NIL
