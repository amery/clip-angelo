static func _outCurrent ( row, hilite, fSay )
   local i,col,vislen,data,datat,srow,cblock
   local colsep,lensep,scol,ecol,len,lenc
   local xcur:=NIL,ycur:=::nLeft,lencur:=0,xcolor,ccolors:="",x,y
#ifdef DEBUG_CALL
	outlog(__FILE__,__LINE__,"_outCurrent",row)
#endif
   if !::__firstStab
	return
   endif
   fSay:=iif(fSay==NIL,.t.,fSay)
   row=iif(row==NIL,::rowPos,row)
   hilite=iif(hilite==NIL,::autoLite,hilite)
   srow=iif(row<1,::rowPos,row)
   if srow<1
	return
   endif
   xcur:=::__rect[1]-1+srow
   if ::winbuffer==nil; dispBegin(); endif
   visLen= len(::__colVisible)
   if fSay .and. hilite .and. ::winbuffer == nil //.or. row==0
	draw_rect(xcur,::nLeft,xcur,::nright,colorToN(::__getcolor(1)))
   endif
   for i=1 to visLen
	  col=::__colVisible[i]
	  colSep:=::__columns[col]:colSep
	  colsep:=iif(colSep==NIL,::colSep,colSep)
	  lenSep:=len(colSep)
	  scol=::__whereVisible[i]
	  ccolors:=::__columns[col]:defcolor
	  //ccolors:=::__colorCells[srow][col]
#ifdef TBR_CACHED
	  if ::__dataCache[sRow][col][1] == NIL
		if ::__columns[col]:fieldName == NIL
			data:=eval(::__columns[col]:block)
		else
			data:=eval(::__columns[col]:block,::__columns[col]:fieldName)
		endif
		::__dataCache[sRow][col][1] := data
		datat:=valtype(data)
		if datat == "U" .and. ::__columns[col]:type != NIL
			datat := ::__columns[col]:type
		endif
		cblock=::__columns[col]:colorBlock
		if valtype(cblock)=="B" //.and. ccolors[1]==1 .and. ccolors[2]==2
			ccolors:=eval(cblock,data)
		endif
		::__dataCache[sRow][col][2] := ccolors
		if valtype(::__columns[col]:picture)=="C"
			data:=transform(data,::__columns[col]:picture)
		elseif valtype(::__columns[col]:len)=="N"
			switch valtype(data)
				case "N"
					data := str(data, ::__columns[col]:len)
					//data := padl( toString(data), ::__columns[col]:len )
				case "U"
					data := space( ::__columns[col]:len )
				otherwise
					data := padr( toString(data), ::__columns[col]:len )
			endswitch
		else
			if valtype(data) == "N"
				data := str(data, ::__columnsLen[col])
			else
				data := toString(data)
			endif
		endif
		::__dataCache[sRow][col][3] := data
	  else
		datat:=valtype(::__dataCache[sRow][col][1])
		ccolors := ::__dataCache[sRow][col][2]
		data := ::__dataCache[sRow][col][3]
	  endif
#else
	  if ::__columns[col]:fieldName == NIL
		data:=eval(::__columns[col]:block)
	  else
		data:=eval(::__columns[col]:block,::__columns[col]:fieldName)
	  endif
	  datat:=valtype(data)
	  if datat == "U" .and. ::__columns[col]:type != NIL
		datat := ::__columns[col]:type
	  endif
	  cblock=::__columns[col]:colorBlock
	  if valtype(cblock)=="B" //.and. ccolors[1]==1 .and. ccolors[2]==2
		ccolors:=eval(cblock,data)
	  endif

	  if valtype(::__columns[col]:picture)=="C"
		data:=transform(data,::__columns[col]:picture)
	  elseif valtype(::__columns[col]:len)=="N"
		switch valtype(data)
			case "N"
				data := str(data, ::__columns[col]:len )
				//data := padl( toString(data), ::__columns[col]:len )
			case "U"
				data := space( ::__columns[col]:len )
			otherwise
				data := toString(data)
		endswitch
	  else
		if valtype(data) == "N"
			if left(str(data),1) == "*"
				data := str(data, ::__columnsLen[col])
			else
				data := str(data)
			endif
		else
			data:=toString(data)
		endif
	  endif
#endif
	  if .t. //!(srow==::rowPos .and. i==::__colPos .and. row>0)
		if !empty(::__colorCells[srow][col])
			ccolors:=::__colorCells[srow][col]
		endif
	  endif
	  len=min(::__columnsLen[col],len(data))
	  lenc=max(::__columnsLen[col],len)
	  len=min(len,::nright-scol)
	  lenc=min(lenc,::nright-scol)
	  ecol=min(scol+::__columnsLen[col],::nright)
	  if vislen==1 .or. i==vislen //.or. vislen==len(::__columns)
		len++; lenc++
	  endif
	  //data:=padr(data,len)
	  data:=left(data,len)
	  xcolor:=::__getcolor(ccolors[1])
	  if hilite .and. row >0
		switch ( ::hiliteType )
		case 0
		case 1
			if srow==::rowPos .and. i==::__colPos .and. row>0
				xcolor:=::__getcolor(ccolors[2])
			endif
		case 2
			if srow==::rowPos
				xcolor:=::__getcolor(ccolors[2])
			endif
		endswitch
	  endif
	  if srow==::rowPos .and. i==::__colPos .and. row>0
		xcur:=::__rect[1]-1+srow
		ycur:=scol
		lencur := len
		if datat $ 'N'
			ycur += lenc-len
		endif
		if datat $ 'L'
			ycur += (lenc-len)/2
		endif
	  endif
	  if fSay
		  if ::winbuffer == nil
			dispOutAt(::__rect[1]-1+srow ,scol,"")
		  endif
		  y := ::__rect[1]-1+srow; x := scol
		  if datat $ "N"
			if ::winbuffer == nil
				dispout(space(lenc-len),::__getcolor(1))
			else
				winbuf_out_at(::winbuffer,y,x,space(lenc-len),::__getcolor(1))
				x += lenc-len
			endif
		  endif
		  if datat $ "L"
			if ::winbuffer == nil
				dispout(space((lenc-len)/2),::__getcolor(1))
			else
				winbuf_out_at(::winbuffer,y,x,space((lenc-len)/2),::__getcolor(1))
				x += (lenc-len)/2
			endif
		  endif
		  if ::winbuffer == nil
			dispOut(data, xcolor)
			//dispOut(space(ecol-col()), xcolor)
			dispOut(space(ecol-col()), ::__getcolor(1))
			if i==vislen //.and. fSay
				dispOut(space(::nRight-col()+1),::__getcolor(1))
			else
				dispOut(colsep,::__getcolor(1))
			endif
		  else
			winbuf_out_at(::winbuffer,y,x,data, xcolor)
			x += len(data)
			//dispOut(space(ecol-col()), xcolor)
			winbuf_out_at(::winbuffer,y,x,space(ecol-x), ::__getcolor(1))
			x := ecol
			if i==vislen //.and. fSay
				winbuf_out_at(::winbuffer,y,x,space(::nRight-col()+1),::__getcolor(1))
			else
				winbuf_out_at(::winbuffer,y,x,colsep,::__getcolor(1))
			endif
		  endif
	  endif
   next
   if hilite .and. ::hiliteType==3 .and. srow==::rowPos .and. row!=0
	xcolor:=colorToN(::__getcolor(ccolors[2]))
	if ::winbuffer == nil
		draw_rect(xcur,::nLeft,xcur,::nright,xColor)
	else
	endif
   endif
   //setpos(xcur,ycur)
   ::cursorCol:=ycur
   ::cursorRow:=xcur
   ::cursorLen:=lencur
   if ::winbuffer==nil; dispOutAt(xcur,ycur,""); endif
   if ::winbuffer==nil; dispEnd(); endif
return NIL
