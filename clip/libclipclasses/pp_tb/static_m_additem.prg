static func m_addItem(self,omenu)
       if valtype(omenu)=="O" //.and. omenu:classname=="MENUITEM"
	  aadd(self:__items,omenu)
	  self:itemCount++
	  self:width:=max(self:width,__capLength(omenu:caption))
       endif
return self
