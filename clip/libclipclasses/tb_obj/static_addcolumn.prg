static func addColumn(self,col)
#ifdef DEBUG_CALL
	outlog(__FILE__,__LINE__,"addColumn")
#endif
	aadd(self:__columns,col)
	self:colCount++
	if self:__firstStab
		self:__remakeColumns()
	endif
return self
