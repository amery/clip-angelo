static func lb_setItem(self,pos,item)
       if pos>0 .and. pos<=len(self:__mText) ;
	  .and. valtype(item)=="A"  .and. len(item)>=2
	  self:__mText[pos]:=item[1]
	  self:__mData[pos]:=item[2]
       endif
return self
