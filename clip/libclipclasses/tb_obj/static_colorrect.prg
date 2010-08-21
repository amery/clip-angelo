static func colorRect(self,rect,block)   /////  ?????????
	local i,j,m1,m2,m3,m4
#ifdef DEBUG_CALL
	outlog(__FILE__,__LINE__,"colorRect",block,rect)
#endif
	if !(valtype(rect)=="A" .and. len(rect)==4)
		return self
	endif
	if empty(block)
		block:={1,2}
	endif
#ifdef __1
	m1:=min(max(1,rect[1]),len(self:__colorCells))
	m2:=min(max(1,rect[3]),len(self:__colorCells))
	for i=m1 to m2
		m3:=min(max(1,rect[2]),len(self:__colorCells[i]))
		m4:=min(max(1,rect[4]),len(self:__colorCells[i]))
		for j=m3 to m4
			self:__colorCells[i][j]:=block
		next
		self:_outCurrent(i,.t.)
	next
#else
	dispBegin()
	m1:=max(1,rect[1])
	m2:=max(1,rect[3])
	for i=m1 to m2
		if len(self:__colorCells) <i
			aadd(self:__colorCells,{})
		endif
		m3:=max(1,rect[2])
		m4:=max(1,rect[4])
		for j=m3 to m4
			if len(self:__colorCells[i]) <j
				aadd(self:__colorCells[i],{1,2})
			endif
			self:__colorCells[i][j]:=block
		next
		self:_outCurrent(i,.t.)
	next
	dispEnd()
#endif
return self
