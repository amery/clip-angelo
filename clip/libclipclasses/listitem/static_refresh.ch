static func refresh()
local cur, i, j, nl:=::nLeft
       if ::union
		::__refreshUnion()
		return
       endif
       if ::line > ::nBottom
	   if ::pos != ::colWin
	       ::pos ++
	       ::line := ::nTop
	   else
	       ::first ++
	       ::line := ::nBottom
	   endif
       endif
       if ::line < ::nTop
	   if ::pos != 1
	       ::pos --
	       ::line := ::nBottom
	   else
	       ::first --
	       ::line := ::nTop
	   endif
       endif

       if ::pos < 1
	   ::pos = 1
	   ::first -= ::rowWin
       endif
       if ::pos > ::colWin
	   ::pos = ::colWin
	   ::first += ::rowWin
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

       dispbegin()
       if ::itemCount == 0
		dispOutAt(::nTop, nl, replicate(" ", ::lencol[1]), ::__colors[2] )
       endif
       for i=1 to ::colWin
	   for j=::nTop to ::nBottom
	       if ::buffer == cur .and. ::itemCount >= cur
		   if !empty(::findbuffer)
			dispOutAt(j, nl, substr(padr(::item[cur]:item, ::lenCol[i]), 1, len(::findbuffer)), iif(::hasFocus, ::__colors[7], iif(::item[cur]:select, ::__colors[5], ::__colors[3])) )
			dispOutAt(j, nl+len(::findbuffer), substr(padr(::item[cur]:item, + ;
			::lenCol[i]), len(::findbuffer)+1), iif(::hasFocus, iif(::item[cur]:select, ::__colors[6], ::item[cur]:scolor), iif(::item[cur]:select, ::__colors[5], ::__colors[3])) )
		   else
			dispOutAt(j, nl, padr(::item[cur]:item, ::lenCol[i]), iif(::hasFocus, iif(::item[cur]:select, + ;
			::__colors[6], ::item[cur]:scolor), iif(::item[cur]:select, ::__colors[5], ::__colors[3])) )
		   endif
		   ::pos := i
		   ::line := j
	       else
		   if cur <= ::itemCount
		       dispOutAt(j, nl, padr(::item[cur]:item, ::lenCol[i]),;
			   iif(::item[cur]:select, ::__colors[5], ::item[cur]:color) )
		   else
		       dispOutAt(j, nl, padl(" ", ::lenCol[i]), ::__colors[1] )
		   endif
	       endif
	       if i!=::colWin
		   dispOutAt(j, nl+::lenCol[i], ::delim, ::__colors[1] )
	       endif
	       cur ++
	   next
	   nl += ::lenCol[i]+1
       next
       if empty(::findbuffer)
	   ::col := ::nLeft
	   for i=1 to ::pos-1
		::col += ::lencol[i]+1
	   next
       else
	   ::col:= ::nLeft+len(::findbuffer)
	   for i=1 to ::pos-1
		::col += ::lencol[i]+1
	   next
       endif
       devpos(::line, ::col)
       dispend()
return
