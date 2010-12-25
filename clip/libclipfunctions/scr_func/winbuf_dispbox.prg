Function winbuf_dispbox(winbuffer,top, left, bottom, right, xType,color)
	local ch,ch1,ch2,x,y,sStyle
	//local d_string :=[��������], s_string :=[��������]
	local d_string := B_DOUBLE
	local s_string := B_SINGLE
	if valtype(xType)=="N"
		sStyle:=iif(xType==1,s_string,d_string)
	else
		sStyle:=iif(valtype(xType)=="C",xType,B_SINGLE)
		//sStyle := strtran(sStyle,B_DOUBLE,d_string)
		//sStyle := strtran(sStyle,B_SINGLE,s_string)
	endif
	bottom:=max(top,bottom)
	right:=max(left,right)
	if len(sStyle)<8
		padr(sStyle,8,left(sStyle,1))
	endif
	if len(sStyle)>8
		ch:=replicate(substr(sStyle,9,1),right-left+1)
		for x=top to bottom
			winbuf_out_at(winbuffer,x,left,ch,color)
		next
	endif
	if top==bottom
		winbuf_out_at(winbuffer,top,left,replicate(substr(sStyle,2,1),right-left+1),color)
	elseif left==right
		ch:=substr(sStyle,8,1)
		for x=top to bottom
			winbuf_out_at(winbuffer,x,left,ch,color)
		next
	else
		ch1:=substr(sStyle,8,1)
		ch2:=substr(sStyle,4,1)
		for x=top to bottom
			winbuf_out_at(winbuffer,x,left,ch1,color)
			winbuf_out_at(winbuffer,x,right,ch2,color)
		next
		winbuf_out_at(winbuffer,top,left,replicate(substr(sStyle,2,1),right-left+1),color)
		winbuf_out_at(winbuffer,bottom,left,replicate(substr(sStyle,6,1),right-left+1),color)
		winbuf_out_at(winbuffer,top,left,substr(sStyle,1,1),color)
		winbuf_out_at(winbuffer,top,right,substr(sStyle,3,1),color)
		winbuf_out_at(winbuffer,bottom,left,substr(sStyle,7,1),color)
		winbuf_out_at(winbuffer,bottom,right,substr(sStyle,5,1),color)
	endif
return
