static func lb_prevPage(self)
	if self:value>1
	   self:value -= (self:bottom-self:top)
	   if self:isopen
	      if self:value<1
		 self:__line:=min(1,self:itemCount)
	      endif
	   endif
	   self:value:=min(max(1,self:value),self:itemCount)
	endif
	self:__setBuffer()
	self:display()
return self
