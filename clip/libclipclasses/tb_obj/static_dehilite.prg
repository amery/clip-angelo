static func deHilite(self)
#ifdef DEBUG_CALL
	outlog(__FILE__,__LINE__,"dehilite")
#endif
	self:_outCurrent(NIL,.f.)
return self
