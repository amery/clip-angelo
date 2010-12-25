static func home(self)
#ifdef DEBUG_CALL
	outlog(__FILE__,__LINE__,"Home")
#endif
	self:hitTop:=self:hitBottom:=.f.
	self:__colPos:=min(max(1,self:freeze+1),self:colCount)
	self:colpos:=self:__colVisible[self:__colpos]
	self:_outCurrent()

return self
