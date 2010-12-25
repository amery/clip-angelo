static func m_setItem(self,npos,omenu)
       if npos>0 .and. npos<=self:itemCount
	  if valtype(omenu)=="O" //.and. omenu:classname=="MENUITEM"
	     self:__items[npos]:=omenu
	     self:width:=max(self:width,__capLength(omenu:caption))
	  endif
       endif
return self
