static function _display ()
  local bpos,epos,spos,mlen,p,ssay,i,x,y,s
  local setd:=set(_SET_DELIMITERS),setdc:=set(_SET_DELIMCHARS)
#ifdef DEBUG
	outlog(__FILE__,__LINE__,"_display",::name)
#endif
  if ::control != NIL
	return
  endif
  if ::__first
	::setfocus()
	::killfocus()
  endif
  if ::winbuffer == nil
	dispbegin()
  endif
  //if ::caption!=NIL
  if !empty(::caption)
     __sayCaption(::capRow,::capCol,::caption,::__colors[1])
  endif
  if !::hasFocus
      if ::winbuffer == nil
		dispOutAt(::row, ::col, iif(setd,substr(setdc,1,1),""), ::__colors[1] )
      else
		s:= iif(setd,substr(setdc,1,1),"")
		winbuf_out_at(::winbuffer,::row,::col,s, ::__colors[1])
		y := ::row; x := ::col + len(s)
      endif
      //ssay:=substr(::buffer,1,::__winlen)
      ssay:= ::__toString()
      if "Q" $ ::__flags
	 ssay=padr(replicate("*",len(rtrim(ssay))),len(ssay))
      endif
      if ::winbuffer == nil
		dispOut(ssay,::__colors[5])
		dispOut(iif(setd,substr(setdc,2,1),""),::__colors[1])
      else
		winbuf_out_at(::winbuffer,y,x,ssay, ::__colors[5])
		x += len(ssay)
		s:= iif(setd,substr(setdc,2,1),"")
		winbuf_out_at(::winbuffer,y,x,s, ::__colors[1])
		y := ::row; x := ::col + len(s)
      endif
      //setpos(::row,::col)
      if ::winbuffer == nil
		dispend()
      endif
      return NIL
  endif
  spos:=int(::__winLen/4)
  epos:=len(::__format)
  epos=min(::pos+spos,epos)
  epos=max(epos,::__winLen)
  bpos=epos-::__winLen+1
  if ::winbuffer == nil
	dispOutAt(::row, ::col, iif(setd,substr(setdc,1,1),""), ::__colors[2] )
  else
	s := iif(setd,substr(setdc,1,1),"")
	winbuf_out_at(::winbuffer,::row, ::col, s, ::__colors[2] )
	y := ::row; x := ::col + len(s)
  endif
  ssay:=substr(::buffer,bpos,epos-bpos+1)
  if "Q" $ ::__flags
	 ssay=padr(replicate("*",len(rtrim(ssay))),len(ssay))
  endif
  ssay := padr(ssay, ::__winlen)
  if ::winbuffer == nil
	dispOut(ssay,::__colors[2])
	dispOut(iif(setd,substr(setdc,2,1),""),::__colors[2])
  else
	winbuf_out_at(::winbuffer,y,x,ssay,::__colors[2])
	x += len(ssay)
	winbuf_out_at(::winbuffer,y,x,iif(setd,substr(setdc,2,1),""),::__colors[2])
  endif
  ::cursorPos := ::__winLen-(epos-::pos)-iif(setd,0,1)+1
  if ::winbuffer == nil
	setpos(::row,::col+::cursorPos-1)
	dispend()
  endif
return NIL
