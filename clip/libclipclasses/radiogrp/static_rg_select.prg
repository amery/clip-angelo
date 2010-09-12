static func rg_select(self,newState)
	local i
       if newstate!=NIL
          if valtype(newState) == "N"
    		self:__setValue(newState)
          else
		for i=1 to len(self:__items)
                	if valtype(self:__items[i]:rvalue) == valtype(newState) ;
                        	.and. self:__items[i]:rvalue == newState

    				self:__setValue(i)
                        endif

		next
          endif
       endif
       self:display()
return self
