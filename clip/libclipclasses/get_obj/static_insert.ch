static function Insert(sym)
   local s,p,s1,s2
#ifdef DEBUG
	outlog(__FILE__,__LINE__,"insert",::name)
#endif
   ::typeOut:=.f.
   p:=ascan(::__posArr,::pos)
   if p<1 .or. (p>len(::__original) .and. !::expand) .or. ::readOnly
      ::__firstKey:=.t.
      return NIL
   endif
   ::rejected:=.f.

   s:=::__checkSym(sym)

   if !::__firstKey .and. "K"$::__flags .and. ::type!="L"
      ::__original:=space(len(::__original))
      ::changed:=.t.
   endif
   if len(s)<=0
      ::rejected:=.t.
      return NIL
   endif
   if ::type=="L"
	if s==" "
		::__original:=iif(left(::__original,1)==left(LANG_YES_STRING,1),LANG_NO_STRING,LANG_YES_STRING)
	else
		::__original:=iif( (upper(s) $ ("TY"+LANG_YES_CHAR) ),LANG_YES_STRING,LANG_NO_STRING)
	endif
	::__dataSay:=::__original
   elseif ::type=="C"
	if "R" $ ::__flags //::type=="C"
		if !::expand .and. p!=len(::__posArr)
			::__original:=left(::__original,len(::__original)-1)
		endif
		::__original:=substr(::__original,1,p-1)+s+substr(::__original,p)
	else
		//::__original:=substr(::__original,1,::pos-1)+s+substr(::__original,::pos)
		__get_split_by_picture(::__posarr,p,::__original,@s1,@s2)
		s1:=left(s1,len(s1)-1)
		::__original:=substr(s1,1,::pos-1)+s+substr(s1,::pos)+s2
	endif
     else
	::__original:=substr(::__original,1,p-1)+s+substr(::__original,p+1)
   endif
   ::__firstKey:=.t.
   ::__fillBuffer()
   ::changed:=.t.
   if ::type != "L"
	::right()
   else
	::typeout := .t.
	::_display()
   endif
return NIL
