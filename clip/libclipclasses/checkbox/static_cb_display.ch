static func cb_display(self)
      local pal
      pal:=self:__colors[iif(self:hasFocus,2,1)]
       dispOutAt(self:row, self:col, substr(self:style,1,1)+" "+substr(self:style,4,1),self:__colors[3] )
      dispOutAt(self:row, self:col+1, substr(self:style,iif(self:buffer,2,3),1), pal)
      __sayCaption( self:capRow, self:capCol, self:caption, pal,,self:__colors[4] )
      dispOutAt(self:row, self:col+1, "" )
return self
