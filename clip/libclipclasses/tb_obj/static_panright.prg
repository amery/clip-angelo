static func panRight(self)
	local i,j,k
	if self:__rightAll
		return self
	endif
	i=self:colpos
	k=self:__colPos
	self:__colPos=len(self:__colVisible)
	self:colPos=self:__colVisible[self:__colPos]
	self:__whoVisible(1)
	self:__colPos=k
	for j=1 to len(self:__colVisible)
		if i = self:__colVisible[j]
			self:__colPos=j
			exit
		endif
	next

	self:colPos=self:__colVisible[self:__colPos]
	self:refreshAll()

return self
