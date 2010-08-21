static func lb_nextPage(self)
	if self:value<self:itemCount
	   self:value += (self:bottom-self:top)
	   if self:isopen
	      if self:value > self:itemCount
		self:__line := self:itemcount
	      endif
	   endif
	   self:value:=min(self:value,self:itemCount)
	endif
	self:__setBuffer()
	self:display()
return self
