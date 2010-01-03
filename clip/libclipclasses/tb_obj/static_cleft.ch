static func cleft(self)

	self:hitTop:=self:hitBottom:=.f.
	if self:__colpos==1
		if self:__colVisible[1] > 1
			self:__whoVisible(-1)
			self:refreshAll()
		endif
	else
		self:__colPos--
		if len(self:__colVisible) > self:freeze .and. ;
		    self:__colVisible[self:freeze+1] > self:freeze+1 .and. ;
		    self:__colPos <= self:freeze
			self:__colPos++
			self:__whoVisible(-1)
			self:refreshAll()
		else
			self:colpos:=self:__colVisible[self:__colpos]
			self:_outCurrent()
		endif
	endif
return self
