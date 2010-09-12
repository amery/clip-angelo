static func pp_hitTest(mrow,mcol)
	local ret:=0,i,x,item
	if ! ( mCol>::left .and. mCol < (::left+::width)+2 )
		return 0
	endif
	for i=::__begItem to ::itemCount
		x:=::top+i-::__begItem+1
		item:=::__items[i]
		if item:caption!=MENU_SEPARATOR
			if x==mRow
				ret:=i
				exit
			endif
		endif
	next
return ret
