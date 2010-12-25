static func rg_setStyle(self,newStyle)
       local i
       if newStyle==NIL
	  return self
       endif
       for i=1 to self:itemCount
	   self:__items[i]:style:=newStyle
       next
       self:display()
return self
