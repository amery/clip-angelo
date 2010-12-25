static func goBottom(self)
#ifdef DEBUG_CALL
	outlog(__FILE__,__LINE__,"goBottom")
#endif
	self:hitTop:=self:hitBottom:=.f.
	if self:gobottomBlock!=NIL
		self:rowPos=self:rowCount
		eval(self:gobottomBlock)
		self:refreshAll()
	endif
return self
