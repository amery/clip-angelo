static func lb_nextItem(self)
	if self:value<self:itemCount
	   self:value++
	   if self:isopen
	      self:__line++
	   endif
	endif
	self:__setBuffer()
	self:display()
return self
