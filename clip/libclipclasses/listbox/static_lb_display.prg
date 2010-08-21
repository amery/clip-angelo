static function lb_display(self)
  local curr:=self:value, pal:=1, ntop:=self:top,i,ltop,atop
  if len(self:__mtext) == 0
	return self
  endif
  self:value:=min(self:itemCount,max(self:value,1))
  if self:value>self:itemCount .or. self:value<1
	return self
  endif
  dispbegin()
  if self:dropDown
      pal:= iif(self:hasFocus,2,1)
      dispOutAt(self:top, self:left, ;
	    padr(self:__mtext[curr],self:right-self:left)+ LB_PUSHSYM,;
	    self:__colors[pal] )
      ntop++
  endif
  if self:isopen
     if self:hasFocus
		if self:hotBox!=NIL
			@ ntop, self:left, self:bottom, self:right box ;
				self:hotBox color self:__colors[3]
		else
			@ ntop, self:left TO self:bottom, self:right;
				DOUBLE color self:__colors[3]
		endif
     else
		if self:coldBox!=NIL
			@ ntop, self:left, self:bottom, self:right box ;
				self:coldBox color self:__colors[3]
		else
			@ ntop, self:left TO self:bottom, self:right;
				 color self:__colors[3]
		endif
     endif
     self:__line:=min(min(max(1,self:__line),self:bottom-ntop-1),self:itemCount)
     ltop:=self:value-self:__line+1
     ltop:=min(ltop,self:itemcount - ( self:bottom-ntop-2) )
     ltop:=max(ltop,1)
     //pal:= iif(self:hasFocus,2,5)
     pal:=5
     for i=1 to self:bottom-ntop-1
	 if ltop<=self:itemCount
	    dispOutAt(ntop+i, self:left+1,;
		  padr(self:__mText[ ltop ],self:right-self:left-1),;
		  self:__colors[pal] )
	 else
	    dispOutAt(ntop+i, self:left+1, space(self:right-self:left-1) )
	 endif
	 ltop++
     next
     pal:= iif(self:hasFocus,2,1)

     dispOutAt(ntop+self:__line, self:left+1,;
	     padr(self:__mText[ self:value ],self:right-self:left-1),;
	     self:__colors[pal] )
  endif

  if self:__SB != NIL .and. self:isopen
     self:__SB:total  := self:itemCount
     self:__SB:current:= curr
     self:__SB:display()
  endif
  dispend()
return self
