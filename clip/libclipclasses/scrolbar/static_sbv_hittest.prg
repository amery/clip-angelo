static func sbv_hitTest(self,mouseRow,mouseCol)
       local ret:=0, pos:=mouseRow
       if mouseCol==self:offset
	  if pos>self:start .and. pos<self:start+self:thumbPos
	     ret=HTSCROLLBLOCKDEC
	  endif
	  if pos>self:start+self:thumbPos .and. pos<self:end
	     ret=HTSCROLLBLOCKINC
	  endif
	  if pos==self:start+self:thumbPos
	     ret=HTSCROLLTHUMBDRAG
	  endif
	  if pos==self:start
	     ret=HTSCROLLUNITDEC
	  endif
	  if pos==self:end
	     ret=HTSCROLLUNITINC
	  endif
       endif
return ret
