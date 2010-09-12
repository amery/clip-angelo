static function __setcolor()
local i, s
       s:=::colorSpec
       while len(s)>0
	   i:=at(",",s)
	   i=iif(i==0,len(s)+1,i)
	   aadd(::__colors,substr(s,1,i-1) )
	   s:=substr(s,i+1)
       enddo
return
