static function __refreshUnion()
local i,a, j, nl:=::nLeft, s, es:="", cur
       if ::line > ::nBottom
	       ::first ++
       endif
       if ::line < ::nTop
	       ::first --
       endif

       if ::pos > 1
	   ::pos = 1
	   ::first += ::rowWin
       endif
       if ::pos < 1
	   ::pos = 1
	   ::first -= ::rowWin
       endif

       ::first := min(::first, ::itemCount)
       ::first := max(::first, 1)
       ::buffer := min(::buffer, ::itemCount)
       ::buffer := max(::buffer, 1)
       cur := ::first

       ::line := min(::line, ::nTop+(::buffer-::first)%::rowWin)
       if !empty(::findbuffer)
	   ::line := ::nTop+(::buffer-::first)%::rowWin
       endif
       for i=1 to ::colWin
		es += padr(" ", ::lenCol[i]) + iif(i<::colWin, ::delim, "")
       next
       dispbegin()
       if ::itemCount == 0
	   dispOutAt(::nTop, nl, es, ::__colors[2] )
       endif
       for j=::nTop to ::nBottom
	   if cur > ::itemCount
		dispOutAt(j, nl, es, ::__colors[1] )
		loop
	   endif
	   s:=""
	   a := {}
	   a := split(::item[cur]:item, ::delim)
	   asize(a, ::colWin)
	   for i=1 to ::colWin
		s += padr(a[i], ::lenCol[i]) + iif(i<::colWin, ::delim, "")
	   next
	   if ::buffer == cur
		dispOutAt(j, nl, s, iif(::hasFocus, iif(::item[cur]:select, ::__colors[6], ::item[cur]:scolor), iif(::item[cur]:select, ::__colors[5], ::__colors[3])) )
		::pos := 1
		::line := j
	   else
		dispOutAt(j, nl, s, iif(::item[cur]:select, ::__colors[5], ::item[cur]:color) )
	   endif
	   cur ++
       next

       if empty(::findbuffer)
	   ::col := ::nLeft
	   for i=1 to ::pos
		::col += ::lencol[i]
	   next
       else
	   ::col:= ::nLeft+len(::findbuffer)
	   for i=1 to ::pos
		::col += ::lencol[i]
	   next
       endif
       devpos(::line, ::col)
       dispend()
return
