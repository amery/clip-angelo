static func lb_getItem(pos)
       if pos>0 .and. pos<=len(::__mText)
	  return {::__mText[pos],::__mData[pos]}
       endif
return NIL
