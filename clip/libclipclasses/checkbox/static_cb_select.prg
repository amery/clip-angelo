static func cb_select(self,newState)
       if newstate==NIL
	  self:buffer:=!self:buffer
	  eval(self:sblock)
       elseif valtype(newState) == "L"
	  self:buffer:=newState
       endif
       self:display()
return self
