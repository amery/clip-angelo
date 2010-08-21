static function __setcolor()
local i, s, j:=1
       s:=::colorSpec
       asize(::__colors, 10)
       afill(::__colors, "0/7")
       while len(s)>0
	   i:=at(",",s)
	   i=iif(i==0,len(s)+1,i)
	   ::__colors[j] := substr(s,1,i-1)
	   s:=substr(s,i+1)
	   j++
       enddo
return
