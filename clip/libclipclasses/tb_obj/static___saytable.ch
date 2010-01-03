static func __sayTable
   local visLen,col,colSep,lensep,lenhsep
   //local strsep1,strsep2
   local strseph,scol,len,strings
   local i,j,k,s,ccc,headsep,strhsep1, x,y
   local heading:=.t., footing:=.t.
#ifdef DEBUG_CALL
	outlog(__FILE__,__LINE__,"sayTable")
#endif
   if ::winbuffer==nil; dispBegin(); endif

   visLen= len(::__colVisible)
   j=0;k=0
   for i=1 to visLen
	col=::__colVisible[i]
	headSep:=::__columns[col]:headSep
	if headSep!=NIL .and. headSep==""
	   j++
	endif
	headSep:=::__columns[col]:footSep
	if headSep!=NIL .and. headSep==""
	   k++
	endif
   next
   heading := !(j==visLen)
   footing := !(k==vislen)
   for i=1 to visLen
	col=::__colVisible[i]
	len=::__columnsLen[col]
	colSep:=::__columns[col]:colSep
	colsep:=iif(colSep==NIL,::colSep,colSep)
	lenSep:=len(colSep)
	//strsep1:=iif(lenSep>2,substr(colsep,3,1),"")
	//lenSep:=iif(lenSep==3,3,1)
	//strsep1:=space(len(strsep1))
	//strsep2:=substr(colsep,1,2)
	//strsep2:=space(len(strsep2))
	scol=::__whereVisible[i]
	//len=min(len,::nright-scol-lenSep+len(strSep2))
	if vislen !=1
		len=min(len,::nright-scol-lenSep)
	endif
	strings:=::__HeadStrings[col]
	ccc:=::__getcolor(1)
	if ::__columns[col]:defColor!=NIL
		ccc:=::__getcolor(::__columns[col]:defColor[1])
	endif
	if ::__columns[col]:colorHeading!=NIL
		ccc:=::__getcolor(::__columns[col]:colorHeading)
	endif
	for j=1 to len(strings )
		dispOutAt(::nTop+j-1, scol,space(::nRight-scol),::__getcolor(1))
		s=padr(strings[j],len)
		y := ::nTop+j-1
		x := scol
		dispOutAt(::nTop+j-1, scol,"")
		if ::__columns[col]:colSepH != nil
			strseph:=substr(::__columns[col]:colSepH,j,1)
			if ::winbuffer == nil
				dispOutAt(::nTop+j-1, scol,strseph,::__getcolor(1))
			else
				winbuf_out_at(::winbuffer,::nTop+j-1, scol,strseph,::__getcolor(1))
			endif
			x := scol + len(strseph)
		else
			if ::winbuffer == nil
				//dispOutAt(::nTop+j-1, scol,colsep,::__getcolor(1))
			else
				//winbuf_out_at(::winbuffer,::nTop+j-1, scol,strsep1,::__getcolor(1))
			endif
			//x := scol + len(strsep1)
			x := scol + len(colsep)
		endif
		if ::winbuffer == nil
			dispOut(s,ccc)
			//dispOut(iif(i<visLen,colSep,""),::__getcolor(1))
			//dispOut(iif(i<visLen,strSep2,""),::__getcolor(1))
		else
			winbuf_out_at(::winbuffer,y,x,s,ccc)
			x += len(s)
			//winbuf_out_at(::winbuffer,y,x,iif(i<visLen,strSep2,""),::__getcolor(1))
			//x += len(iif(i<visLen,strSep2,""))
		endif
		if i==visLen
			if ::winbuffer == nil
				dispout(space(::nRight-col()+1),::__getcolor(1))
			else
				winbuf_out_at(::winbuffer,y,x,space(::nRight-col()+1),::__getcolor(1))
			endif
		endif
	next
	headSep:=::__columns[col]:headSep
	headSep:=iif(headSep==NIL,::headSep,headSep)
	lenHSep=len(headSep)
	strhsep1=right(headsep,1)
	j:=len(strings)
	if heading .and. ((j>0 .and. lenHSep>0 ) .or. !empty(::headSep))
		if ::__columns[col]:colSepH != nil .and. len(::__columns[col]:colSepH)>j
			strseph:=substr(::__columns[col]:colSepH,j+1,1)
			if ::winbuffer == nil
				dispOutAt(::nTop+j, scol-1, strseph,::__getcolor(1))
			else
				winbuf_out_at(::winbuffer,::nTop+j, scol-1, strseph,::__getcolor(1))
			endif
		endif
		//s := replicate(strHsep1,len+lenSep-lenHsep)+iif(i<visLen,headsep,"")
		if i==1
			s := replicate(strHsep1,len)
			x := scol
		else
			s := headsep+replicate(strHsep1,len+lenSep-lenHsep)
			x := scol-lenSep
		endif
		if ::winbuffer == nil
			dispOutAt(::nTop+j, x,s, ::__getcolor(1))
		else
			winbuf_out_at(::winbuffer,::nTop+j, x, s, ::__getcolor(1))
		endif
		y := ::nTop+j; x := scol + len(s)
		if i==vislen
			if ::winbuffer == nil
				dispout(replicate(strHsep1,::nright-col()+1),::__getcolor(1))
			else
				winbuf_out_at(::winbuffer,y,x, replicate(strHsep1,::nright-col()+1),::__getcolor(1))
			endif
		endif
		j++
	endif
	::__rect[1]=::nTop+j
	strings:=::__footStrings[col]
	ccc:=::__getcolor(1)
	if ::__columns[col]:defColor!=NIL
		ccc:=::__getcolor(::__columns[col]:defColor[1])
	endif
	if ::__columns[col]:colorFooting!=NIL
		ccc:=::__getcolor(::__columns[col]:colorFooting)
	endif
	for j=1 to len(strings)
		s=padr(strings[j],len+lenSep)
		//outlog(__FILE__,__LINE__,strings[j],s,len,ccc)
		if ::winbuffer == nil
			//dispOutAt(::nBottom-j+1, scol, strsep1, ::__getcolor(1))
			dispOutAt(::nBottom-j+1, scol, s, ccc)
			//dispOut(s, ccc)
			//dispOut(iif(i<visLen,strSep2,"") , ::__getcolor(1))
			if i==visLen
				dispout(space(::nRight-col()+1),::__getcolor(1))
			endif
		else
			//winbuf_out_at(::winbuffer,::nBottom-j+1, scol, strsep1, ::__getcolor(1))
			y := ::nBottom-j+1
			//x := scol + len(strsep1)
			winbuf_out_at(::winbuffer,y,x,s,ccc)
			x += len(s)
			//winbuf_out_at(::winbuffer,y,x,iif(i<visLen,strSep2,"") , ::__getcolor(1))
			//x += len(iif(i<visLen,strSep2,""))
			if i==visLen
				winbuf_out_at(::winbuffer,y,x,space(::nRight-col()+1),::__getcolor(1))
			endif
		endif
	next
	//if !empty(::footSep)
	headSep:=::__columns[col]:footSep
	headsep:=iif(headSep==NIL,::footSep,headSep)
	lenHSep=len(headSep)
	strhsep1=right(headsep,1)
	//endif
	j:=len(strings)
	//if ( j>0 .and. lenHSep>0 ) .or. !empty(::footSep)
	if footing .and. lenHSep>0  .and. !empty(headSep)
		//s := replicate(strHsep1,len)+iif(i<visLen,headsep,"")
		if i==1
			s := replicate(strHsep1,len)
			x := scol
		else
			s := headsep+replicate(strHsep1,len+lenSep-lenHsep)
			x := scol-lenSep
		endif
		if ::winbuffer == nil
			dispOutAt(::nBottom-j, x, s , ::__getcolor(1))
			if i==vislen
				dispOut(replicate(strHsep1,::nright-col()+1),::__getcolor(1))
			endif
		else
			winbuf_out_at(::winbuffer,::nBottom-j, x, s, ::__getcolor(1))
			y := ::nBottom-j+1
			x := scol + len(s)
			if i==vislen
				winbuf_out_at(::winbuffer,y,x,replicate(strHsep1,::nright-col()+1),::__getcolor(1))
			endif
		endif
		j++
	endif
	::__rect[3]=::nBottom-j
   next
   ::rowCount:=::__rect[3]-::__rect[1]+1
   if ::winbuffer==nil
	dispEnd()
   endif
   ::__lRedrawTable := .f.

return NIL
