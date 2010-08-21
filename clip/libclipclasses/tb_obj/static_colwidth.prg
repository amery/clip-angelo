static func colWidth(self,col)
	local ret:=0,x
#ifdef DEBUG_CALL
	outlog(__FILE__,__LINE__,"colWidth")
#endif
	x=ascan(self:__colVisible,col)
	if x>0
		ret=self:__columnsLen[col]
	endif
return ret
