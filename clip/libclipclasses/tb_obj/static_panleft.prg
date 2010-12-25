static func panLeft(self)
	local i,j,k
	if self:freeze >=len(self:__colVisible)
		return self
	endif
	if self:__colVisible[self:freeze+1] == self:freeze+1
		return self
	endif

	i=self:colPos
	k=self:__colPos
	self:__colPos=self:freeze+1
	self:colPos=self:__colVisible[self:__colPos]
	self:__whoVisible(-1)
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
