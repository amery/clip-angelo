static func rb_select(self,newState)
       if newstate==NIL
	  self:buffer:=.t.
       else
	  self:buffer:=newState
       endif
       self:display()
return self
