static func rg_nextItem(self)
    if self:value>0 .and. self:value<=self:itemCount
       self:__items[self:value]:select(.f.)
       self:__items[self:value]:killFocus()
    endif
    self:value++
    self:value:=min(self:itemCount,self:value)
    self:__setValue(self:value)
    if self:value>0 .and. self:value<=self:itemCount
       self:__items[self:value]:select(.t.)
       self:__items[self:value]:setFocus()
    endif
    self:display()
return self
