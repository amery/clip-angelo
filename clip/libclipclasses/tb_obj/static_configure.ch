static func configure(self)
#ifdef DEBUG_CALL
	outlog(__FILE__,__LINE__,"configure")
#endif
	 self:stable := .f.
	 self:__firstStab := .f.
	 self:__setColor()
return self
