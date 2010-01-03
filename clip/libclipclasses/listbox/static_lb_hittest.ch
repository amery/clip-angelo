static func lb_hitTest(mrow,mcol)
       local ret:=0, ntop
       if ::isopen .and. ::__SB!=NIL
	  ret=::__SB:hitTest(mrow,mcol)
	  if ret!=0
	     return ret
	  endif
       endif
       ntop:=iif(::dropDown,::top+1,::top)
       if ::isopen
	     if mrow==ntop
		if mcol>::left .and. mcol<::right
		   ret=HTTOP
		endif
		if mcol==::left
		   ret=HTTOPLEFT
		endif
		if mcol==::right
		   ret=HTTOPRIGHT
		endif
	     endif
	     if mrow==::bottom
		if mcol>::left .and. mcol<::right
		   ret=HTBOTTOM
		endif
		if mcol==::left
		   ret=HTBOTTOMLEFT
		endif
		if mcol==::right
		   ret=HTBOTTOMRIGHT
		endif
	     endif
	     if mcol==::left .and. mrow>ntop .and. mrow<::bottom
		ret=HTLEFT
	     endif
	     if mcol==::right .and. mrow>ntop .and. mrow<::bottom
		ret=HTRIGHT
	     endif
	     if mrow>ntop .and. mrow<::bottom .and. ;
		    mcol>::left .and. mcol<::right
		ret=HTCAPTION
	     endif
       endif
       if ::dropDown
	   if mrow==::top .and. mcol>=::left .and. mcol<::right
		ret=HTCAPTION
	   endif
	   if mrow==::top .and. mcol==::right
		ret=HTDROPBUTTON
	   endif
       endif
return ret
