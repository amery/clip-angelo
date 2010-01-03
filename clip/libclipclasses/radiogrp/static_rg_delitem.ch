static func rg_delItem(self,pos)
   adel(self:__items,pos)
   self:itemCount:=max(len(self:__items)-1,0)
   if self:itemCount==0
      self:typeOut:=.t.
   endif
   asize(self:__items,self:itemCount)
return self
