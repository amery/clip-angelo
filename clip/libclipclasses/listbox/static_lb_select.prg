static func lb_select(self,pos)
	if valtype(pos)=="N"
	   self:value:=pos
	   self:value:=min(max(1,self:value),self:itemCount)
	   self:__setBuffer()
	   self:display()
	   if self:sblock != NIL
		eval(self:sblock)
	   endif
	endif
return  self
