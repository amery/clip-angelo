static func lb_prevItem(self)
	if self:value>1
	   self:value--
	   if self:isopen
	      self:__line--
	   endif
	endif
	self:__setBuffer()
	self:display()
return self
