static func lb_getText(pos)
       if pos>0 .and. pos<=len(::__mText)
	  return ::__mText[pos]
       endif
return NIL
