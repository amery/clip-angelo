static func __dummyRow( row )
   local i,col,vislen,colBlock,data,colsep
   local srow,lensep,scol,len,x,y
#ifdef DEBUG_CALL
	outlog(__FILE__,__LINE__,"__dummyRow")
#endif
   row=iif(row==NIL,::rowPos,row)
   srow=iif(row<1,::rowPos,row)
   if ::winbuffer==nil; dispBegin(); endif
   visLen= len(::__colVisible)
   for i=1 to visLen
	  col=::__colVisible[i]
	  colBlock:=::__columns[col]:block
	  colSep:=::__columns[col]:colSep
	  colsep:=iif(colSep==NIL,::colSep,colSep)
	  lenSep:=len(colSep)
	  scol=::__whereVisible[i]
	  len=::__columnsLen[col]
	  len=min(len,::nright-scol)
	  if ::winbuffer == nil
		dispOutAt(::__rect[1]-1+srow ,scol,space(len), ::__getcolor(1))
		if i==vislen
			dispOut(space(::nRight-col()+1),::__getcolor(1))
		else
			dispOut(colsep,::__getcolor(1))
		endif
	  else
		winbuf_out_at(::winbuffer,::__rect[1]-1+srow ,scol,space(len), ::__getcolor(1))
		y := ::__rect[1]-1+srow; x := scol + len
		if i==vislen
			winbuf_out_at(::winbuffer,y,x,space(::nRight-col()+1),::__getcolor(1))
		else
			winbuf_out_at(::winbuffer,y,x,colsep,::__getcolor(1))
		endif
	  endif
   next
   if ::winbuffer==nil; dispEnd(); endif
return NIL
