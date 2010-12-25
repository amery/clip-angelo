static func rg_insItem(self,pos,obj_rb)
   self:itemCount++
   asize(self:__items,self:itemCount)
   ains(self:__items,pos,obj_rb)
   self:typeOut:=.f.
   self:display()
return self
