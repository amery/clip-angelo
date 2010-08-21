static function delRight()
   local p,s1,s2,s3
#ifdef DEBUG
	outlog(__FILE__,__LINE__,"delright",::name)
#endif
   p:=ascan(::__posArr,::pos)
   ::__firstKey:=.t.
   if p<1 .or. p>len(::__original) .or. ::readOnly
      return NIL
   endif
   if ::type=="C"
      //::__original=substr(::__original,1,p-1)+substr(::__original,p+1)+" "
      __get_split_by_picture(::__posarr,p,::__original,@s1,@s2)
      ::__original=substr(s1,1,::pos-1)+substr(s1,::pos+1)+" "+s2
   endif
   if ::type=="D"
      ::__original=substr(::__original,1,p-1)+" "+substr(::__original,p+1)
   endif
   if ::type=="T"
      ::__original=substr(::__original,1,p-1)+" "+substr(::__original,p+1)
   endif
   if ::type=="N"
	if ::decpos>0
		s1:=substr(::__original,1,::decpos-1)
		s2:=substr(::__original,::decpos)
		if p<::decpos
			s1=substr(s1,1,p-1)+substr(s1,p+1)+" "
		else
			s2=substr(s2,1,p-::decpos)+substr(s2,p-::decpos+2)+" "
		endif
		::__original=s1+s2
	else
		::__original=substr(::__original,1,p-1)+substr(::__original,p+1)+" "
	endif
   endif
   ::changed:=.t.
   ::__fillBuffer()
   ::_display()
return NIL
