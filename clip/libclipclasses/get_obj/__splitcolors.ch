function __splitColors(s)
       local i,__colors:={},ss
       if s==NIL .or. empty(s)
	  return __colors
       endif
       s:=upper(s)
       while len(s)>0
	   i:=at(",",s)
	   i=iif(i==0,len(s)+1,i)
	   ss:=alltrim(substr(s,1,i-1))
	   aadd(__colors, ss )
	   s:=substr(s,i+1)
       enddo
       if empty(__colors)
		aadd(__colors,"W+/B")
       endif
       for i=len(__colors) to GET_MAX_COLORS
	   aadd(__colors,__colors[1])
       next
return __colors
