static func hitTest(mouseRow,mouseCol)
    local ret:=HTNOWHERE
    if mouseRow==::row
       if mouseCol==::col+1
	  ret=HTCLIENT
       else
	  if mouseCol>=::capCol .and. mouseCol<=::capCol+len(::caption)
	     ret=HTCAPTION
	  endif
       endif
    endif
return ret
