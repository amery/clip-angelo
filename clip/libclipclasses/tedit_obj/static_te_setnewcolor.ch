static function te_setNewColor(newColor)
local i, oldcolor
       if newColor == NIL
		return
       endif
       oldcolor := ::__colors
       asize(::__colors, 0)
       ::colorSpec := iif(newColor!=NIL, newColor, ::colorSpec)
       ::__setcolor()
       i := len(::__colors)
       if i < len(oldcolor)
	  asize(::__colors, len(oldcolor))
	  for i:=i+1 to len(oldcolor)
		::__colors[i] := oldcolor[i]
	  next
       endif
       ::refresh()
return
