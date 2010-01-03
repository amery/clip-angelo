static func __setcolor()
local s, i
       ::__colors:={}
       s:=::colorSpec
       while len(s)>0
	   i:=at(",",s)
	   i=iif(i==0,len(s)+1,i)
	   aadd(::__colors,substr(s,1,i-1) )
	   s:=substr(s,i+1)
       enddo
       i := len(::__colors)
       if i<7
	   asize(::__colors, 7)
	   afill(::__colors, "7/0", i+1)
       endif
return NIL
