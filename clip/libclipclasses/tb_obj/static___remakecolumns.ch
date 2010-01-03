static func __remakeColumns
   local a,s,i,j,h,f,hr,hf,max_hr,max_hf,clen
   local data
#ifdef DEBUG_CALL
	outlog(__FILE__,__LINE__,"remakeColumns")
#endif
   if !::__firstStab
   //   return NIL
   endif
   ::__headStrings:={}
   ::__footStrings:={}
   max_hr:=0; max_hf:=0
   for i=1 to len(::__columns)
	  h:=::__columns[i]:heading
	  //hr:=iif(empty(h),0,occurs(";",h)+1)
	  hr:=iif(h==NIL .or. h=="",0,occurs(";",h)+1)
	  max_hr:=max(max_hr,hr)
	  f:=::__columns[i]:footing
	  hf:=iif(f==NIL .or. f=="",0,occurs(";",f)+1)
	  max_hf:=max(max_hf,hf)
   next
   ::__columnsLen:={}
   for i=1 to len(::__columns)
	  clen:=0
	  aadd(::__headStrings,{})
	  h:=::__columns[i]:heading
	  //hr:=iif(empty(h),0,occurs(";",h)+1)
	  hr:=iif(h==NIL .or. h=="",0,occurs(";",h)+1)
	  for j=1 to hr
		a=at(";",h)
		a=iif(a==0,len(h)+1,a)
		s=substr(h,1,a-1)
		clen:=max(clen,len(s))
		aadd(::__headStrings[i], s )
		h:=substr(h,a+1)
	  next
	  for j=hr+1 to max_hr
		aadd(::__headStrings[i],"")
	  next

	  aadd(::__footStrings,{})
	  f:=::__columns[i]:footing
	  hf:=iif(f==NIL .or. f=="",0,occurs(";",f)+1)
	  for j=1 to hf
		a=at(";",f)
		a=iif(a==0,len(f)+1,a)
		s=substr(f,1,a-1)
		clen=max(clen,len(s))
		aadd(::__footStrings[i],s)
		f:=substr(f,a+1)
	  next
	  for j=hf+1 to max_hf
		aadd(::__footStrings[i],"")
	  next
	  if !empty(::__columns[i]:width) .and. valtype(::__columns[i]:width)=="N"
		clen:=::__columns[i]:width
	  elseif valtype(::__columns[i]:picture)=="C" .or. valtype(::__columns[i]:len)!="N"
		if ::__columns[i]:fieldname == NIL
			data=eval(::__columns[i]:block)
		else
			data=eval(::__columns[i]:block,::__columns[i]:fieldname)
		endif
		if valtype(::__columns[i]:picture)=="C"
			data:=transform(data,::__columns[i]:picture)
		else
			data:=toString(data)
		endif
		clen:=max(clen,len(data))
	  else
		clen:=max(clen,::__columns[i]:len)
	  endif

	  aadd(::__columnsLen,clen)
   next
   ::__headRows:=max_hr-iif(empty(::headSep),1,0)
   ::__footRows:=max_hf
   ::__rect:={::ntop+::__headRows,::nleft,::nbottom-::__footRows,::nright}
   //outlog(__FILE__,__LINE__,::__rect)
   ::rowCount:=::__rect[3]-::__rect[1]+1
   //::__colorCells:={}
   asize(::__colorCells,::rowCount)
   for i=1 to len(::__colorCells)
	::__colorCells[i]={}
	asize(::__colorCells[i],len(::__columns))
	afill(::__colorCells[i],NIL)
	//afill(::__colorCells[i],{1,2})
	/*
	for j=1 to len(::__colorCells[i])
		::__colorCells[i][j]:= ::__columns[j]:defColor
	next
	*/
   next
#ifdef TBR_CACHED
   asize(::__dataCache,::rowCount)
   for i=1 to len(::__dataCache)
	::__dataCache[i]={}
	asize(::__dataCache[i],len(::__columns))
	for j=1 to len(::__dataCache[i])
		::__dataCache[i][j]={}
		asize(::__dataCache[i][j],3)
		afill(::__dataCache[i][j],NIL)
	next
   next
#endif

    ::__refreshLine:=array(::rowCount)
    afill(::__refreshLine,0)

return NIL
