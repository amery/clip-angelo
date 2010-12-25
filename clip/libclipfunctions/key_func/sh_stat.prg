function sh_stat()
	local key:=kbdstat()
	if key==1 .or. key==2
		return .f.
	endif
return .t.
