static func refreshCurrent(self)
	local i,j
#ifdef DEBUG_CALL
	outlog(__FILE__,__LINE__,"refreshCurrent")
#endif
	i:=self:rowpos
	self:hitTop:=self:hitBottom:=.f.
	if !self:__firstStab .or. i < 1
		return self
	endif
	self:__refreshLine[i] := 0
	//afill(self:__colorCells[self:rowPos],{1,2})
	afill(self:__colorCells[i],NIL)
	/*
	for j=1 to len(self:__colorCells[i])
		self:__colorCells[i][j]:=self:__columns[j]:defColor
	next
	*/
	self:stable:=.f.
return self
