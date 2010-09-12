static func goTop(self)
#ifdef DEBUG_CALL
	outlog(__FILE__,__LINE__,"goTop")
#endif
	self:hitTop:=self:hitBottom:=.f.
	if self:gotopBlock!=NIL
		self:rowPos=1
		eval(self:gotopBlock)
		self:refreshAll()
	endif
return self
