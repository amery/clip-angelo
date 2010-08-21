static func cright(self)

	self:hitTop:=self:hitBottom:=.f.
	if self:__colpos==len(self:__colVisible)
		if ! self:__rightAll
			self:__whoVisible(1)
			self:refreshAll()
		endif
	else
		self:__colPos++
		self:colpos:=self:__colVisible[self:__colpos]
		self:_outCurrent()
	endif
return self
