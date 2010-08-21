static function delWordLeft()
   local p,p1:=0,p2:=0,ch,len
#ifdef DEBUG
	outlog(__FILE__,__LINE__,"delWordLeft",::name)
#endif
   ::__firstKey:=.t.
   ::changed:=.t.
   p:=ascan(::__posArr,::pos)
   ::__firstKey:=.t.
   len:=len(::__original)
   if p<1 .or. p>len .or. ::readOnly
	return NIL
   endif
   if ::type!="C"
	return NIL
   endif
   p--
   ch := substr(::__original,p,1)
   while ch==" " .and. p>0
	p--
	ch := substr(::__original,p,1)
   end
   p1:=p
   p++
   ch := substr(::__original,p,1)
   while !(ch==" ") .and. p<=len
	p++
	ch := substr(::__original,p,1)
   end
   while ch==" " .and. p<=len
	p++
	ch := substr(::__original,p,1)
   end
   p2:=p-1
   p:=p1
   ch := substr(::__original,p,1)
   while !(ch==" ") .and. p>0
	p--
	ch := substr(::__original,p,1)
   end
   p++

   ::__original=padr(substr(::__original,1,p-1)+substr(::__original,p2+1),len)
   if p>0 .and. p<=len(::__posArr)
      ::pos:=::__posArr[p]
   endif
   ::changed:=.t.
   ::__fillBuffer()
   ::_display()
return NIL
