function restcursor(m)
	if valtype(m)=="O"
		if "ROW" $ m .and. "COL" $ m
			devpos(m:row,m:col)
		endif
		if "CURSOR" $ m
			setcursor(m:cursor)
		endif
		if "CCURSOR" $ m
			csetcurs(m:ccursor)
		endif
	endif
return
