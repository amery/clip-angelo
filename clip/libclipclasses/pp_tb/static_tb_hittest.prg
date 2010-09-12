static func tb_hitTest(mrow,mcol)
	local ret:=0,i,j,xpos:=::left,item
	if mRow != ::row
		return 0
	endif
	for i=::__begitem to ::itemCount
		item:=::__items[i]
		j:=xpos+__capLength(item:caption)+2
		if mcol > xpos .and. mcol < j
			ret:=i
		endif
		if xpos>::right
			exit
		endif
		xpos+=__capLength(item:caption)+2
	next
return ret
