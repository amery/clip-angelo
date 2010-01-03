static function cur_right()
   local p
#ifdef DEBUG
	outlog(__FILE__,__LINE__,"cur_right",::name)
#endif
   p:=ascan(::__posArr,::pos)
   p++
   if p>len(::__posArr)
	if ::expand
		::pos:=atail(::__posArr)+1
		aadd(::__posArr,::pos)
		::__format+="X"
		if ::__maxlen > 0
			::__winLen++
			::__winLen:=min(::__maxLen,::__winLen)
		endif
	else
		::typeOut:=.t.
	endif
	p:=len(::__posArr)
   endif
   p:=max(1,p)
   if p>0 .and. p<=len(::__posArr)
		::pos:=::__posArr[p]
   endif
   ::__firstKey:=.t.
   ::_display()
return NIL
