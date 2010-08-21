static func pb_display(self)
      local pal,s
      pal:=self:__colors[iif(self:hasFocus,2,1)]
      if self:style==NIL
		if self:winbuffer == nil
			@ self:row, self:col TO self:row+2, self:col+1+__capLength(self:caption) ;
				color pal
		else
			winbuf_dispbox(self:winbuffer, self:row, self:col, ;
				self:row+2, self:col+1+__capLength(self:caption), 1, pal)
		endif
		__sayCaption( self:Row+1, self:Col+1, self:caption, pal,,self:__colors[4],self:winbuffer)
      else
	if len(self:style)<=2
		s=substr(self:style,1,1)+self:caption+substr(self:style,2,1)
		__sayCaption( self:Row, self:Col, s, pal,,self:__colors[4],self:winbuffer)
		if self:winbuffer == nil
			dispOutAt(self:row, self:col+1, "")
		endif
	else
		if self:winbuffer == nil
			@ self:row, self:col, self:row+2, self:col+1+__capLength(self:caption) ;
			box self:style color pal
		else
			winbuf_dispbox(self:row, self:col, self:row+2, ;
			self:col+1+__capLength(self:caption), self:style, pal)
		endif
		__sayCaption( self:Row+1, self:Col+1, self:caption, pal,,self:__colors[4],self:winbuffer)
	endif
      endif
return self
