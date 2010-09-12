static function wordRight()
   local p,fl:=.f.,s
#ifdef DEBUG
	outlog(__FILE__,__LINE__,"wordright",::name)
#endif
   p:=ascan(::__posArr,::pos)
   while p<len(::__original)
	s=substr(::__original,p,1)
	if s!=" "
	   if fl
	      exit
	   endif
	else
	   fl:=.t.
	endif
	p++
   enddo
   ::__firstKey:=.t.
   if p>0 .and. p<=len(::__posArr)
      ::pos:=::__posArr[p]
      ::_display()
   endif
return NIL
