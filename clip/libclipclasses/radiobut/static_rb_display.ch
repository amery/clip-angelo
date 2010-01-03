static func rb_display(self)
      local pal,s
      pal:=self:__colors[iif(self:hasFocus,2,1)]
      s=substr(self:style,1,1)
      s+=substr(self:style,iif(self:buffer,2,3),1)
      s+=substr(self:style,4,1)
      dispOutAt(self:row, self:col, s, pal )
      __sayCaption( self:row, self:col+4, self:caption, self:__colors[1],,;
      		self:__colors[iif(self:hasFocus,6,7)])
      setpos(self:row, self:col+1)
return self
