static func pageUp(self)
	local xskip,nskip
#ifdef DEBUG_CALL
	outlog(__FILE__,__LINE__,"pageUp")
#endif
	self:hitTop:=self:hitBottom:=.f.
	xskip=0-self:rowCount
	nskip=eval(self:skipBlock,xskip)
	if nskip<>0
		if nskip>xskip
			self:hitTop := .t.
			self:rowPos:=self:rowpos-nskip+xskip
			self:rowpos:=max(1,self:rowpos)
		endif
		self:refreshAll()
	endif
return self
