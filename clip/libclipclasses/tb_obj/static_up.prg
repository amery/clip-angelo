static func up(self)
	local oldrow,nskip

	self:hitTop:=self:hitBottom:=.f.
	nskip:=eval(self:skipBlock,-1)
	if nskip==0
		self:hitTop:=.t.
	else
		if self:rowpos<2
			self:refreshAll()
		else
			self:refreshCurrent()
			self:rowpos--
			self:refreshCurrent()
		endif
	endif
return self
