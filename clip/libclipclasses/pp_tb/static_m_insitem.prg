static func m_insItem(self,npos,omenu)
       if npos>0 .and. npos<=self:itemCount
	  //if valtype(omenu)=="O" //.and. omenu:classname=="MENUITEM"
	     asize(self:__items,len(self:__items)+1)
	     self:__items[npos]:=omenu
	     self:itemCount++
	 //    self:width:=max(self:width,__capLength(omenu:caption))
	  //endif
       endif
return self
