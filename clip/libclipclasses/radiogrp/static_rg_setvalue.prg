static func rg_setValue(self,newValue)
	if newValue >0 .and. newValue <=len(self:__items)
                if valtype(self:__items[newValue]:rValue) != NIL
                	self:buffer := self:__items[newValue]:rValue
                else
                	self:buffer := newValue
                endif
                self:value := newValue
        else
        	self:value := 0
        	self:buffer := 0
        endif
