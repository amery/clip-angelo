static func panEnd(self)

	if self:__colpos <> len(self:__colVisible) .or.	! self:__rightAll
		self:__colpos:=self:colcount+1
		self:__whoVisible(self:__colPos)
		self:__colpos:=len(self:__colVisible)
		self:colPos:=self:__colVisible[self:__colPos]
		self:refreshAll()
	endif

return self
