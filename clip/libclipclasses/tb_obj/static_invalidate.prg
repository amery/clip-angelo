static func invalidate(self)
#ifdef DEBUG_CALL
	outlog(__FILE__,__LINE__,"invalidate")
#endif
	 self:__setColor()
	 self:__remakeColumns()
	 self:__whoVisible()
	 self:refreshAll()
return self
