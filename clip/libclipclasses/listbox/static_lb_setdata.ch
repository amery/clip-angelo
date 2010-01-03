static func lb_setData(self,pos,data)
       if pos>0 .and. pos<=len(self:__mData)
	  self:__mData[pos]:=data
       endif
return self
