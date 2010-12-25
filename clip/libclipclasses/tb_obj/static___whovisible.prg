static func __whoVisible(num)
	   local i,cnum,leftcol,scol,oldcol,col,mincol,maxcol
#ifdef DEBUG_CALL
	outlog(__FILE__,__LINE__,"whoVisible",num)
#endif
	   if empty(::__columns)
		return
	   endif
	   cnum=iif(num==NIL,0,num)

	   ::freeze:=max(::freeze,0)
	   ::freeze:=min(::freeze,::colCount)
	   if (empty(::__colVisible))
		col    := 1  // ::colpos
		oldcol := ::colpos
	   else
		if num == NIL
			col := ::__colVisible[1]
		else
			col:=min(max(::__colPos,::freeze+1),::colCount)
			if col <= len(::__colVisible)
				col:=::__colVisible[col]
			endif
		endif
		oldcol:=max(1,min(::colCount,::colPos))
	   endif
	   scol:=::__rect[2]
	   leftcol:=scol
	   for i=1 to ::freeze
		leftcol+=::__columnsLen[i]+::__lenColSep(i)
	   next
	   while i<=::colCount .and. leftcol>::nRight-3 .and. ::freeze>0
		leftcol-=::__columnsLen[i]+::__lenColSep(i)
		::freeze--
	   enddo
	   leftcol:=max(leftcol,::__rect[2])
	   ::__colVisible:={}
	   ::__whereVisible:={}
	   for i=1 to ::freeze
		aadd(::__colVisible,i)
		aadd(::__whereVisible,scol)
		scol+=::__columnsLen[i]+::__lenColSep(i)
	   next
	   *
	   col+=cnum
	   col=min(max(min(col,::colCount),::freeze+1),::colCount)
	   if cnum>=0 .and. num != NIL
			scol:=::__rect[4]-(::__columnsLen[col]+::__lenColSep(col))
			while scol>leftcol .and. col > ::freeze+1
				col--
				scol-=::__columnsLen[col]+::__lenColSep(col)
			enddo
			if scol<leftcol .and. (col<=oldcol .or. cnum >= ::colCount) .and. cnum>0
				col++
			endif
	   endif
	   mincol:=col
	   scol:=leftcol
	   while scol<::__rect[4] .and. col<=::colCount
		scol+=::__columnsLen[col]+::__lenColSep(col)
		col++
	   enddo
	   scol--
	   col--
	   col=min(col,::colCount)
	   col=max(1,col)
	   maxcol:=col
	   if (col>1 .and. !::chop .and. ::colCount>1 .and. scol-::__lenColSep(col)>::__rect[4] .and. mincol!=maxcol) .or. cnum>::colCount
			if cnum < ::colCount
				col--
				maxcol:=col
			endif
			scol:=::__rect[4]-(::__columnsLen[col]+::__lenColSep(col))
			while scol>leftcol .and. col>1
				col--
				scol-=::__columnsLen[col]+::__lenColSep(col)
			enddo
			if scol+::__lenColSep(col)<leftcol
				col++
			endif
			col:=max(1,col)
			mincol:=col
			if mincol >maxcol
				mincol := maxcol
			endif
	   endif
	   mincol := max(::freeze+1,min(mincol,len(::__columns)))
	   maxcol := min(maxcol,len(::__columns))
	   * add to list of columns
	   scol:=leftcol
	   ::leftVisible := mincol
	   for col=mincol to maxcol
		aadd(::__colVisible,col)
		aadd(::__whereVisible,scol)
		scol+=::__columnsLen[col]+::__lenColSep(col)
	   next
	   if empty(::__colVisible)
		aadd(::__colVisible,col)
		aadd(::__whereVisible,scol)
	   endif
	   ::rightVisible := atail(::__colVisible)
	   for i=1 to len(::__colVisible)
		if ::__colVisible[i]==oldcol+cnum
			col:=::__colVisible[i]
			::__colPos=i
		endif
	   next
	   ::__rightAll := (maxcol==::colcount)
	   ::__colPos:=min(::__colPos,len(::__colVisible))
	   ::__colPos:=max(::__colPos,1)
	   ::colpos:=::__colVisible[::__colpos]
	   ::__lRedrawTable := .t.
return NIL
