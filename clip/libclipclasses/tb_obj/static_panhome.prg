static func panHome(self)

	self:hitTop:=self:hitBottom:=.f.
	if self:__colpos <> 1 .or.  self:__colVisible[1] <> 1
		self:__colpos:=1
		self:__whoVisible(0-self:colCount)
		self:refreshAll()
	endif

return self
