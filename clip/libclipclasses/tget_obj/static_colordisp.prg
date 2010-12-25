static func colorDisp (color)
  if color!=NIL
     ::colorSpec:=color
  endif
  ::__setcolor()
  ::edit:setColor(::colorSpec)
  ::display()
return ::classname
