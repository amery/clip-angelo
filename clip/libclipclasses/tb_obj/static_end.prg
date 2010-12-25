static func end(self)
#ifdef DEBUG_CALL
	outlog(__FILE__,__LINE__,"End")
#endif
	self:hitTop:=self:hitBottom:=.f.
	if self:__colPos <> len(self:__colVisible)
		self:__colPos:=len(self:__colVisible)
		self:colpos:=self:__colVisible[self:__colpos]
		self:_outCurrent()
	endif
return self
