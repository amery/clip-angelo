static func down(self)
	local nskip

	self:hitTop:=self:hitBottom:=.f.
	nskip:=eval(self:skipBlock,1)
	if nskip=0
		self:hitBottom=.t.
	else
		if self:rowpos >= self:rowCount
			self:refreshAll()
		else
			self:refreshCurrent()
			self:rowpos++
			self:refreshCurrent()
		endif
	endif
return self
