function __sayCaption(nrow,ncol,str,ccolor,len,ncolor,winbuffer)
   local s,oldcol,n,n1,n2
#ifdef DEBUG
	outlog(__FILE__,__LINE__,"__sayCaption")
#endif
   if nrow==NIL .or. ncol==NIL .or. str==NIL .or. ccolor==NIL
      return .f.
   endif
   oldcol:=setcolor(ccolor)
   if ncolor==NIL
	ncolor:=setcolor(,.t.)
	n:=at("/",ncolor)
	n1 := val(substr(ncolor,n+1))
	n2 := val(ncolor)
	n2:=numxor(numxor(n1,0x07),n2)
	n2:=numxor(n1+n2,0x07)
	ncolor:=alltrim(str(n2,2,0))+"/"+alltrim(str(n1,2,0))
   endif
   n=at("&",str)
   s:=strtran(str,"&","")
   if len!=NIL
      s:=padr(s,len)
   endif
   if winbuffer == nil
	dispOutAt(nrow,ncol, s )
	if n!=0
		n--
		dispattr(nrow,ncol+n,nrow,ncol+n,ncolor)
	endif
   else
	winbuf_out_at(winbuffer,nrow,ncol,s,ccolor)
	if n!=0
		n--
		winbuf_attr_at(winbuffer,nrow,ncol+n,nrow,ncol+n,ncolor)
	endif
   endif
   setcolor(oldcol)
return .t.
