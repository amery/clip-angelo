static func lb_setText(self,pos,mtext)
       if pos>0 .and. pos<=len(self:__mText)
	  self:__mText[pos]:=mText
       endif
return self
