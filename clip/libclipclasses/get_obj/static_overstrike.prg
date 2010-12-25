static function overStrike(sym)
   local s,p
#ifdef DEBUG
	outlog(__FILE__,__LINE__,"overstrike",::name)
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
     if ::expand .and. p>=len(::__posArr)
		::__original+=" "
     endif

     if "R" $ ::__flags //::type=="C"
	 ::__original:=substr(::__original,1,p-1)+s+substr(::__original,p+1)
     else
	 //::__original:=substr(::__original,1,p-1)+s+substr(::__original,p+1)
	 ::__original:=substr(::__original,1,::pos-1)+s+substr(::__original,::pos+1)
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
