static function cur_end()
   local p
#ifdef DEBUG
	outlog(__FILE__,__LINE__,"cur_end",::name)
#endif
   if len(::__posArr) == 0
	return NIL
   endif
   ::pos:=atail(::__posArr)
   p:=ascan(::__posArr,::pos)
   while p>0
	::pos:=::__posArr[p]
	if !empty(substr(::buffer,::pos,1))
		p++
		exit
	endif
	p--
   end
   p:=max(p,1)
   p:=min(p,len(::__posArr))
   if p>0 .and. p<=len(::__posArr)
      ::pos:=::__posArr[p]
   endif
  ::__firstKey:=.t.
  ::_display()
return NIL
