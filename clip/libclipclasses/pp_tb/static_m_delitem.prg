static func m_delItem(self,npos)
       if npos>0 .and. npos<=self:itemCount
	  adel(self:__items,npos)
	  self:itemCount--
       endif
return self
