static func rg_addItem(self,obj_rb)
   aadd(self:__items,obj_rb)
   self:itemCount++
   self:typeOut:=.f.
   //self:display()
return self
