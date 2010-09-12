static func hilite(self)
#ifdef DEBUG_CALL
	outlog(__FILE__,__LINE__,"hilite")
#endif
	self:_outCurrent(NIL,.t.)
return self
