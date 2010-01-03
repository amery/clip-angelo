static func __setcolor()
	local i,s
#ifdef DEBUG_CALL
	outlog(__FILE__,__LINE__,"setColor")
#endif
	   ::__colors:={}
	   s:=::colorSpec
	   while len(s)>0
		i:=at(",",s)
		i=iif(i==0,len(s)+1,i)
		aadd(::__colors,substr(s,1,i-1) )
		s:=substr(s,i+1)
	   enddo
	   /*
	   if len(::__colors) < 1
		aadd(::__colors,"W/N")
	   endif
	   if len(::__colors) < 2
		aadd(::__colors,nToColor(invertAttr(::__colors[1])))
	   endif
	   */
	   //outlog(__FILE__,__LINE__,::__colors)

	   /*
	   for i=len(::__colors) to 10
		aadd(::__colors,"n/w" /*"W/N" */)
	   next
	   */
return NIL
