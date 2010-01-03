static func lb_scroll(self,num)
       if num==HTSCROLLUNITDEC
	  self:value++
	  if self:isopen
	      self:__line++
	  endif
       endif
       if num==HTSCROLLUNITINC
	  self:value--
	  if self:isopen
	      self:__line--
	  endif
       endif
       if num==HTSCROLLBLOCKDEC
	  self:value+=self:bottom-self:top-iif(self:dropDown,1,0)
       endif
       if num==HTSCROLLBLOCKINC
	  self:value-=self:bottom-self:top-iif(self:dropDown,1,0)
       endif

       self:value:=min(max(1,self:value),self:itemCount)
       self:__setBuffer()
       self:display()

return self
