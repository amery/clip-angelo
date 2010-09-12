static func rg_display(self, sb)
      local i,pal,s
      pal:=self:__colors[iif(self:hasFocus,2,1)]
      sb := iif(sb==NIL, .t., sb)
      dispbegin()
      if sb
      		if self:hasFocus
                	if self:hotBox!=NIL
	 			@ self:top, self:left, self:bottom, self:right;
	 			box self:Hotbox color self:__colors[1]
                        else
	 			@ self:top, self:left TO self:bottom, self:right;
	 			DOUBLE color self:__colors[1]
                        endif
                else
                	if self:coldBox!=NIL
	 			@ self:top, self:left, self:bottom, self:right;
	 			box self:coldBox color self:__colors[1]
                        else
	 			@ self:top, self:left TO self:bottom, self:right;
	 			color self:__colors[1]
                        endif

                endif
      endif
      if self:caption!=NIL
	 dispOutAt(self:top, self:left+2, self:caption, pal)
      endif
      for i=1 to self:itemCount
	  self:__items[i]:display()
      next
      if self:value>0 .and. self:value<=self:itemCount
	self:__items[self:value]:display()
      endif
      dispend()
return self
