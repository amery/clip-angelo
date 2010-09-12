static func pb_select(self,newState)
       if newstate==NIL
	  self:buffer:=.t.
	  eval(self:sblock)
       else
	  self:buffer:=newState
       endif
       self:display()
return self
