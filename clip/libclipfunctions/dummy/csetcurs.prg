function csetcurs(lvisible)
	local ret
	ret:=setcursor()!=0
	if lVisible != NIL
		setcursor(iif(lvisible,1,0))
	endif
return ret
