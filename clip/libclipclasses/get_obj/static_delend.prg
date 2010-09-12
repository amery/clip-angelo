static function delEnd()
   local p,s1,s2,s3
#ifdef DEBUG
	outlog(__FILE__,__LINE__,"delend",::name)
#endif
   p:=ascan(::__posArr,::pos)
   ::__firstKey:=.t.
   if p<1 .or. p>len(::__original) .or. ::readOnly
      return NIL
   endif
   if ::type$"CND"
      ::__original=substr(::__original,1,p-1)+space(len(::__original)-p+1)
   endif
   ::changed:=.t.
   ::__fillBuffer()
   ::_display()
return NIL
