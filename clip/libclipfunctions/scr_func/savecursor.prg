function savecursor()
	local ret:=map()
	ret:row:=row()
	ret:col:=row()
	ret:cursor:=setcursor()
	ret:ccursor:=csetcurs()
return ret
