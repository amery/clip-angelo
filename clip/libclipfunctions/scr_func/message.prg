function message(str,y,x)
	local _maxRowN:=maxRow()/2-2
	local j,oldcolor,scr, row,col
	local oldCur:=setCursor(0)
	if funcname("PROV_BLANK")
		return .t.
	endif
	if y==NIL
		y=_maxRowN
	endif
	row=row()
	col=col()
	save screen to scr
	j=len(str)
	oldcolor=setcolor("W/R")
	x=iif(x==NIL,(80-j-2)/2,x)
	Dbox(y,x,y+2,x+j+3)
	dispOutAt( y+1,x+2, str, "W+/R")
	inkey(max(len(str)/30,1))
	clearkey()
	setcolor(oldcolor)
	restore screen from scr
	setpos(row,col)
	setcursor(oldCur)
return .t.
