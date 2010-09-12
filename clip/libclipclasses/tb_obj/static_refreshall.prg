static func refreshAll(self)
	local i,j,tmp
#ifdef DEBUG_CALL
	outlog(__FILE__,__LINE__,"refreshAll")
#endif

	self:hitTop:=self:hitBottom:=.f.
	if(  self:__firstStab )
		afill(self:__refreshLine,0)
	endif
	self:stable:=.f.
	   for i=1 to len(self:__colorCells)
		tmp := self:__colorCells[i]
		afill(tmp,NIL)
		/*
		for j=1 to len(tmp)
			tmp[j]:=self:__columns[j]:defColor
		next
		*/
	   next

return self
