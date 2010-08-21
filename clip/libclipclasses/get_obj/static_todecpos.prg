static function toDecPos()
   local i,pos,s1,s2,neg:=.f.
#ifdef DEBUG
	outlog(__FILE__,__LINE__,"todecpos",::name)
#endif
   if ! ( ::type=="N" .and. ::decpos!=NIL )
      return NIL
   endif
   if len(::__posarr) == 0
	return NIL
   endif
   pos:=::pos
   for i=1 to len(::__posArr)
	  if ::__posArr[i]>::decpos
	     ::pos:=::__posArr[i]
	     exit
	  endif
   next
   if !::__firstKey
      ::__original:=str(0,::__len,::__dec)
      ::__original:=strtran(::__original,".","")
      ::__fillBuffer()
   endif
   if pos==::pos
     ::pos:=atail(::__posArr)
   endif
   if ::decpos!=0
	neg := (left(alltrim(::__original),1)=="-")
	s1:=left(::__original,len(::__original)-::__dec)
	s2:=right(::__original,::__dec)
	s2:=strtran(s2," ","0")
	s1:=val(s1,len(s1),0)
	neg := (neg .and. s1==0)
	s1:=str(s1,len(s1),0)
	if neg
		::__original='-'+right(s1,len(s1)-1)+s2
		::__original=strtran(::__original,' ','0')
	else
		::__original=s1+s2
	endif
	::__fillBuffer()
   else
   //  		::__original=substr(::__original,1,p-1)+substr(::__original,p+1)+" "
   endif
  ::__firstKey:=.t.
  ::changed:=.t.
  ::_display()
return NIL
