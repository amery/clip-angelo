static func lb_open(self)
       if self:dropDown
	  self:__scr:= savescreen(self:top+iif(self:dropDown,1,0),self:left,self:bottom,self:right)
	  self:isopen:=.t.
	  self:__line:=self:value
	  self:display()
       endif
return self
