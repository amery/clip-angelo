function inkeytrap(nDelay)
	local ret:=0, bkey
	do while .t.
		ret  := inkey(nDelay)
		bKey := setKey(ret)
		if bKey != NIL
			eval(bKey,procname(1),procline(1),readvar())
			loop
		endif
		exit
	enddo
return ret
