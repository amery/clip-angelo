static func pageDown(self)
	local xskip,nskip
#ifdef DEBUG_CALL
	outlog(__FILE__,__LINE__,"pageDown")
#endif
	self:hitTop:=self:hitBottom:=.f.
	xskip=self:rowCount
	nskip=eval(self:skipBlock,xskip)

	if nskip <> 0
		if nskip < xskip
			self:hitBottom := .t.
			self:rowPos+=nskip
			self:rowPos=max(min(self:rowPos,self:__rect[3]-self:__rect[1]+1),1)
		endif
		self:refreshAll()
	endif
return self
