static func hitTest(mouseRow,mouseCol)
    local ret:=HTNOWHERE
    if mouseRow==::capRow
	  if mouseCol>=::capCol .and. mouseCol<=::capCol+len(::caption)
	     ret=HTCAPTION
	  endif
    endif
return ret
