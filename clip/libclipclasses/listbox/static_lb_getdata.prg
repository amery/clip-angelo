static func lb_getData(pos)
       if pos>0 .and. pos<=len(::__mData)
	  return ::__mData[pos]
       endif
return NIL
