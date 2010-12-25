static func __setcolor()
       ::__colors:=__splitColors(::colorSpec)
       dispOutAt(::row, ::col, substr(::style,1,1) )
       dispOutAt(::row, ::col+2, substr(::style,4,1)+" " )
return NIL

