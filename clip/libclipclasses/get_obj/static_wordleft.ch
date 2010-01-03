static funct wordLeft()
   local p,fl:=.f.,pl:=.f.,s
#ifdef DEBUG
	outlog(__FILE__,__LINE__,"wordleft",::name)
#endif
   p:=ascan(::__posArr,::pos)
   p--
   while p>0
	s=substr(::__original,p,1)
	if s!=" "
	   pl:=.t.
	else
	   if fl .and. pl
	      exit
	   endif
	   fl:=.t.
	endif
	p--
   enddo
   p++
   ::__firstKey:=.t.
   if p>0 .and. p<=len(::__posArr)
      ::pos:=::__posArr[p]
      ::_display()
   endif
return NIL
