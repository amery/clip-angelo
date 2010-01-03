static func __setcolor()
       local i
       ::__colors:=__splitColors(::colorSpec)
       for i=len(::__colors) to 8
	   aadd(::__colors,"W+/B")
       next
return NIL
