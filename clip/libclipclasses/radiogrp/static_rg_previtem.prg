static func rg_prevItem(self)
    if self:value>0 .and. self:value<=self:itemCount
       self:__items[self:value]:select(.f.)
       self:__items[self:value]:killFocus()
    endif
    self:value--
    self:value:=max(1,self:value)
    self:__setValue(self:value)
    if self:value>0 .and. self:value<=self:itemCount
       self:__items[self:value]:select(.t.)
       self:__items[self:value]:setFocus()
    endif
    self:display()
return self
