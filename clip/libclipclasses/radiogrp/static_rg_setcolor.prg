static func rg_setColor(self,newcolor)
       if newcolor!=NIL
	  self:colorSpec:=newColor
	  self:__setcolor()
	  self:display()
       endif
return self
