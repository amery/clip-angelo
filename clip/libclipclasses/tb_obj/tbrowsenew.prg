func TBrowseNew(Lrow,Lcol,Rrow,Rcol,db)
	   local obj

#ifdef DEBUG_CALL
	outlog(__FILE__,__LINE__,"browseNew")
#endif
	   lRow := iif(lRow == NIL, 0, lRow)
	   lCol := iif(lCol == NIL, 0, lCol)
	   rRow := iif(rRow == NIL, MaxRow(), rRow)
	   rCol := iif(rCol == NIL, MaxCol(), rCol)
	   obj		:= map()
	   obj:nTop	:=Lrow
	   obj:nLeft	:=Lcol
	   obj:nBottom	:=Rrow
	   obj:nRight	:=Rcol
	   obj:classname:= "TBROWSE"
	   obj:autoLite	:= .t.
	   obj:cargo	:= NIL
	   obj:colCount	:= 0
	   obj:colorSpec:= setcolor()
	   obj:colPos	:= 1
	   obj:rowPos	:= 1
	   obj:mColPos	:= 0
	   obj:mRowPos	:= 0
	   obj:rowcount	:= rRow-lrow+1
	   obj:colSep	:= " "
	   obj:headSep	:= ""
	   obj:footSep	:= ""
	   obj:freeze	:= 0
	   obj:leftVisible	:= 1
	   obj:rightVisible	:= 1
	   obj:message	:= ""
	   obj:chop		:= .f. /* ���������� ����������� ��������� ������� ?*/
	   obj:hiliteType	:= 1   /* 0 -none
					  1 - current ceil
					  2 - current line
					  3 - current line include separators
					*/
	   obj:refreshBlock	:= {|| NIL }
	   if db!=NIL .and. db
		  obj:goBottomBlock	:= {||dbgobottom() }
		  obj:goTopBlock	:= {|| dbgoTop() }
		  obj:bofBlock		:= {||bof() }
		  obj:eofBlock		:= {||eof() }
		  obj:skipBlock		:= {|x,y,z| __tdbskip(x,y,z) }
	   else
		obj:bofBlock		:= NIL
		obj:eofBlock		:= NIL
		obj:goBottomBlock	:= NIL
		obj:goTopBlock	:= NIL
		obj:SkipBlock		:= NIL
	   endif
	   obj:hitBottom	:=.f.
	   obj:hitTop	:=.f.
	   obj:hitEmpty	:=.f.
	   obj:stable	:=.f.
	   obj:cursorCol:=0
	   obj:cursorRow:=0
	   obj:cursorLen:=0

	  *************************************
	  obj:__firstStab	:=.f.  // ��� �� ������ stabilize
	  obj:__rect	:={0,0,0,0}

	  obj:__colorCells	:={}  // items for colorRect()
#ifdef TBR_CACHED
	  obj:__dataCache	:={}  // cache for output data
#endif
	  obj:__columns	:={}

	  obj:__lRedrawTable    := .t. // need redraw headers & footers
	  obj:__columnsLen	:={} // ����� �������
	  obj:__colVisible	:={} // ������� �������
	  obj:__colpos		:=1  // ������� ������� �� ����� �������
	  obj:__whereVisible	:={} // row ��� ������ ������� �������
	  obj:__headStrings	:={} // ������� ���������� �� �������
	  obj:__footStrings	:={} // ������� �������� �� �������

	  obj:__colors	:={} // ������� ������
	  obj:__keys	:=map() // array for ::setkey()

	  obj:__refreshLine     :={}
	  obj:__headRows	:=0  // ����� � ���������
	  obj:__footRows	:=0  // ����� � �������

	  obj:__rightAll	:=.f. // ������ ������ ������ ���

	  obj:winbuffer		:= NIL

	  _recover_tbrowse(obj)

return obj
