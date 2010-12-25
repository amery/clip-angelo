Function dispboxSay(top, left, bottom, right, xType,color)
	local ch,ch1,ch2,x,y,sStyle
	local row:=row(),col:=col()
	//local d_string :=[��������], s_string :=[��������]
	local d_string := B_DOUBLE
	local s_string := B_SINGLE
	dispbegin()
	if valtype(xType)=="N"
		sStyle:=iif(xType==1,s_string,d_string)
	else
		sStyle:=iif(valtype(xType)=="C",xType,B_SINGLE)
		//sStyle := strtran(sStyle,B_DOUBLE,d_string)
		//sStyle := strtran(sStyle,B_SINGLE,s_string)
	endif
	if empty(Color)
		color := setcolor()
	endif
	bottom:=max(top,bottom)
	right:=max(left,right)
	if len(sStyle) == 0
		sStyle := space(9)
	elseif len(sStyle)<8
		sStyle := padr(sStyle,8,left(sStyle,1))
	endif
	if len(sStyle)>8
		ch:=replicate(substr(sStyle,9,1),right-left+1)
		for x=top to bottom
			dispoutat(x,left,ch,color)
		next
	endif
	if top==bottom
		dispoutat(top,left,replicate(substr(sStyle,2,1),right-left+1),color)
	elseif left==right
		ch:=substr(sStyle,8,1)
		for x=top to bottom
			dispoutat(x,left,ch,color)
		next
	else
		ch1:=substr(sStyle,8,1)
		ch2:=substr(sStyle,4,1)
		for x=top to bottom
			dispoutat(x,left,ch1,color)
			dispoutat(x,right,ch2,color)
		next
		dispoutat(top,left,replicate(substr(sStyle,2,1),right-left+1),color)
		dispoutat(bottom,left,replicate(substr(sStyle,6,1),right-left+1),color)
		dispoutat(top,left,substr(sStyle,1,1),color)
		dispoutat(top,right,substr(sStyle,3,1),color)
		dispoutat(bottom,left,substr(sStyle,7,1),color)
		dispoutat(bottom,right,substr(sStyle,5,1),color)
	endif
	setpos(row,col)
	dispend()
return
